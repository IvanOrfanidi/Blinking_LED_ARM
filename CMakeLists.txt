# Version CMake
cmake_minimum_required(VERSION 3.13)

# Project name
project(blinking_led
    VERSION 1.0.1
    LANGUAGES C CXX ASM
)

# Configure files
configure_file(
    project_version.h.in ${CMAKE_CURRENT_BINARY_DIR}/project_version.h
)

# Suffix
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

# Add CMake scripts (includes) into scope
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/arm-cmake-toolchains")

# Option
set(CMAKE_CXX_STANDARD 17)

# Settings device
set(CORTEX "m3")
set(FAMILY "STM32F10X_MD")
set(LINKER_FILE "${CMAKE_CURRENT_LIST_DIR}/GCC_Linker_Files/ST/STM32F103X8_FLASH.ld")

# Source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
    System/Startup/startup_stm32f103c8tx.s
    System/system_stm32f1xx.c
    System/system_stm32f1xx.h
)

# Subdirectorys
add_subdirectory(STM32F10x_Drivers_Lib)
# Need to access files by < >
target_include_directories(STM32F10x_Drivers_Lib INTERFACE ${CMAKE_CURRENT_LIST_DIR})

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

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

# Compiling target
add_executable(${PROJECT_NAME}_${CMAKE_PROJECT_VERSION} ${SOURCES})

# Targets
target_link_libraries(${PROJECT_NAME}_${CMAKE_PROJECT_VERSION} PRIVATE
    STM32F10x_Drivers_Lib
)

target_include_directories(${PROJECT_NAME}_${CMAKE_PROJECT_VERSION} PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Include utils for generate output files
include(${CMAKE_CURRENT_SOURCE_DIR}/arm-cmake-toolchains/utils.cmake)

# Output files
firmware_size(${PROJECT_NAME}_${CMAKE_PROJECT_VERSION})
generate_object(${PROJECT_NAME}_${CMAKE_PROJECT_VERSION} .bin binary)
generate_object(${PROJECT_NAME}_${CMAKE_PROJECT_VERSION} .hex ihex)
