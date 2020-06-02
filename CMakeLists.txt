# Version CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(blinking_led C CXX ASM)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

# Add CMake scripts (includes) into scope
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/arm-cmake-toolchains")

# Set standart and optimization level
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

# Settings device
set(CORTEX "m3")
set(FAMILY "STM32F10X_HD")
set(LINKER_FILE "${CMAKE_CURRENT_LIST_DIR}/GCC_Linker_Files/ST/STM32F103XC_FLASH.ld")

# Source files
file(GLOB_RECURSE SOURCES
  src/*.cpp
  src/Startup/startup_stm32f103rctx.s
  src/System/*.c
  src/System/*.h
  src/STM32F10x_Drivers_Lib/CMSIS/Include/*.h
  src/STM32F10x_Drivers_Lib/src/*.cpp
  src/STM32F10x_Drivers_Lib/inc/*.h
  src/STM32F10x_Drivers_Lib/inc/utils/*.h)

# Compiler flags
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -fstack-usage")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -fstack-usage")

set(CPU_FLAGS "-mcpu=cortex-${CORTEX} -D${FAMILY}")
set(CPU_FLAGS "${CPU_FLAGS} -mthumb -g3 -march=armv7-m -mfloat-abi=soft")
set(COMPILER_FLAGS "-ffreestanding -ffunction-sections -fdata-sections -fsigned-char -fmessage-length=0 -fshort-enums")
add_definitions(${CPU_FLAGS} ${COMPILER_FLAGS})

# Link options
string(REGEX MATCH ".*\.specs.*" has_specs "${CMAKE_EXE_LINKER_FLAGS}")
if(NOT has_specs)
  string(APPEND CMAKE_EXE_LINKER_FLAGS " --specs=nosys.specs")
endif()
string(APPEND CMAKE_EXE_LINKER_FLAGS " ${CPU_FLAGS} -Wl,-T ${LINKER_FILE}")
string(APPEND CMAKE_EXE_LINKER_FLAGS " -Wl,--gc-sections")

# Compiling target
add_executable(${PROJECT_NAME} ${SOURCES})

# Link target
target_link_libraries(${PROJECT_NAME})

# Include utils for generate output files
include(${CMAKE_CURRENT_SOURCE_DIR}/arm-cmake-toolchains/utils.cmake)

# Output files
firmware_size(${PROJECT_NAME})
generate_object(${PROJECT_NAME} .bin binary)
generate_object(${PROJECT_NAME} .hex ihex)
