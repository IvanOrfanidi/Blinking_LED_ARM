#!/bin/bash
mkdir -p build
cd build

cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./arm-cmake-toolchains/arm-gcc-toolchain.cmake ..
cmake --build . -- -j4
