set(CMAKE_BUILD_TYPE Debug)
cmake_minimum_required(VERSION 3.10)
project(sanpie)
set(LIB_SOURCE_FILES
        sol.hpp
        input/Input.cpp
        input/Wiimote.cpp
        DeviceException.cpp
        output/VJoy.cpp
        output/buttons_ref.h
        output/VKeyboard.cpp
        input/MIDISerial.cpp
        input/MIDIDirect.cpp
        output/VMIDI.cpp
        Loop.cpp
        Utils.cpp
        DeviceScanner.cpp
        input/InputFactory.cpp
        output/OutputFactory.cpp
        input/MIDI.cpp
        input/Serial.cpp
        input/Controller.cpp
        )
add_library(libsanpie ${LIB_SOURCE_FILES})

target_compile_features(libsanpie PUBLIC cxx_std_17)
target_include_directories(libsanpie PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(EXE_SOURCE_FILES main.cpp)
add_executable(sanpie ${EXE_SOURCE_FILES})

target_compile_features(sanpie PUBLIC cxx_std_17)

TARGET_LINK_LIBRARIES(libsanpie udev pthread evdev lua rtmidi)

TARGET_LINK_LIBRARIES(sanpie libsanpie)
