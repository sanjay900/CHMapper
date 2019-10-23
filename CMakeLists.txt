set(CMAKE_BUILD_TYPE Debug)
cmake_minimum_required(VERSION 3.10)

project(chmapper)
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})
set(LIB_SOURCE_FILES src/scanner.cpp src/DeviceException.cpp src/input/Input.cpp src/input/WiiController.cpp src/input/PS3.cpp src/input/Raphnet.cpp)
include_directories(/usr/include/libevdev-1.0)
add_library(libchmapper ${LIB_SOURCE_FILES})

target_compile_features(libchmapper PUBLIC cxx_std_17)
target_include_directories(libchmapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(EXE_SOURCE_FILES src/main.cpp)
add_executable(chmapper ${EXE_SOURCE_FILES})

target_compile_features(chmapper PUBLIC cxx_std_17)

TARGET_LINK_LIBRARIES(libchmapper udev evdev ${GTK3_LIBRARIES})

TARGET_LINK_LIBRARIES(chmapper libchmapper)
