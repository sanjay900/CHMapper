#include "WiiController.h"
#include <iostream>
#include <algorithm>
static int buttons[] = {BTN_DPAD_UP, BTN_DPAD_DOWN, BTN_DPAD_LEFT, BTN_DPAD_RIGHT, BTN_SOUTH, BTN_EAST, BTN_NORTH, BTN_WEST, BTN_SELECT, BTN_START, BTN_MODE, BTN_THUMBL, BTN_THUMBR};
void WiiController::add_child(Input *input)
{
    const std::string found_ext = std::string(libevdev_get_name(input->_dev)).substr(std::string("Nintendo Wii Remote ").size());
    std::cout << found_ext << std::endl;
    if (found_ext == "Accelerometer")
    {
        accel = input;
        //Handle guitar detection before accelerometer
        if (isGuitar) {
            accel->axisMap[ABS_RY] = ABS_RY;
            accel->axisMapScaled[ABS_RY] = [](int val) { return std::min((val * 10) + (32768 / 2), 0); };
        }
        return;
    }
    if (found_ext == "IR")
    {
        return;
    }

    extension = input;
    isGuitar = found_ext == "Guitar";
    if (isGuitar)
    {
        if (accel != nullptr)
        {
            accel->axisMap[ABS_RY] = ABS_RY;
            accel->axisMapScaled[ABS_RY] = [](int val) { return std::min((val * 10) + (32768 / 2), 0); };
        }
        input->buttonMap[BTN_1] = BTN_SOUTH;
        input->buttonMap[BTN_2] = BTN_EAST;
        input->buttonMap[BTN_3] = BTN_WEST;
        input->buttonMap[BTN_4] = BTN_NORTH;
        input->buttonMap[BTN_5] = BTN_TL;
        input->axisMap[ABS_X] = ABS_HAT0X;
        input->axisMap[ABS_Y] = ABS_HAT0Y;
        input->axisMap[ABS_HAT1X] = ABS_RX;
        input->axisMapScaled[ABS_HAT1X] = [](int val) { return std::min(val * 3, 32768); };
    }
    else if (accel != nullptr)
    {
        accel->axisMap.erase(ABS_Y);
        accel->axisMapScaled.erase(ABS_Y);
    }
    for (auto button : buttons)
    {
        input->buttonMap[button] = button;
    }
    input->buttonMap[KEY_PREVIOUS] = BTN_SELECT;
    input->buttonMap[KEY_NEXT] = BTN_START;
    input->buttonMap[BTN_TL2] = BTN_TL;
    input->buttonMap[BTN_TR2] = BTN_TR;
    input->buttonMap[KEY_UP] = BTN_DPAD_UP;
    input->buttonMap[KEY_DOWN] = BTN_DPAD_DOWN;
    input->buttonMap[KEY_LEFT] = BTN_DPAD_LEFT;
    input->buttonMap[KEY_RIGHT] = BTN_DPAD_RIGHT;
    if (found_ext == "Classic Controller")
    {
        //Sticks are upside-down (i think)
        input->axisMap[ABS_HAT1X] = ABS_X;
        input->axisMap[ABS_HAT1Y] = ABS_Y;
        input->axisMap[ABS_HAT3Y] = ABS_Z;
        input->axisMap[ABS_HAT2X] = ABS_RX;
        input->axisMap[ABS_HAT2Y] = ABS_RY;
        input->axisMap[ABS_HAT3X] = ABS_RZ;
    }
}