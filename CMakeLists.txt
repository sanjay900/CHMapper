set(CMAKE_BUILD_TYPE Debug)
cmake_minimum_required(VERSION 3.10)
project(chmapper)
set(LIB_SOURCE_FILES src/scanner.cpp src/DeviceException.cpp src/input/Input.cpp src/input/WiiController.cpp src/input/PS3.cpp src/input/Raphnet.cpp)

add_library(libchmapper ${LIB_SOURCE_FILES})

target_compile_features(libchmapper PUBLIC cxx_std_17)
target_include_directories(libchmapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(EXE_SOURCE_FILES src/main.cpp)
add_executable(chmapper ${EXE_SOURCE_FILES})

target_compile_features(chmapper PUBLIC cxx_std_17)

TARGET_LINK_LIBRARIES(libchmapper udev evdev)

TARGET_LINK_LIBRARIES(chmapper libchmapper)
