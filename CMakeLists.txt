set(CMAKE_BUILD_TYPE Debug)
cmake_minimum_required(VERSION 3.10)
project(sanpie)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/input_events.h
        COMMAND cat /usr/include/linux/input.h /usr/include/linux/input-event-codes.h | python ${CMAKE_CURRENT_SOURCE_DIR}/make-event-names.py > $@
)

set(LIB_SOURCE_FILES input_events.h sol.hpp input/Controller.cpp input/Controller.hpp input/Wiimote.cpp input/Wiimote.hpp ControllerException.hpp ControllerException.cpp output/VJoy.cpp output/VJoy.hpp buttons_ref.h output/VKeyboard.cpp input/SER_MIDI.cpp input/MIDI.cpp output/VMIDI.cpp Loop.cpp Loop.hpp Utils.cpp Utils.hpp DeviceScanner.cpp DeviceScanner.hpp input/InputFactory.cpp input/InputFactory.hpp output/OutputFactory.cpp output/OutputFactory.hpp)
add_library(libsanpie ${LIB_SOURCE_FILES})

target_compile_features(libsanpie PUBLIC cxx_std_17)
target_include_directories(libsanpie PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(EXE_SOURCE_FILES main.cpp)
add_executable(sanpie ${EXE_SOURCE_FILES})

target_compile_features(sanpie PUBLIC cxx_std_17)

TARGET_LINK_LIBRARIES(libsanpie udev pthread evdev lua rtmidi)

TARGET_LINK_LIBRARIES(sanpie libsanpie)
