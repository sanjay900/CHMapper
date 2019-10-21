#include "PS3.h"
#include <iostream>
void PS3::init()
{
    Input::init();
    std::cout << "PS3" << std::endl;
    buttonMap[BTN_EAST] = BTN_SOUTH;
    buttonMap[BTN_C] = BTN_EAST;
    buttonMap[BTN_NORTH] = BTN_WEST;
    buttonMap[BTN_SOUTH] = BTN_NORTH;
    buttonMap[BTN_WEST] = BTN_TL;
    buttonMap[BTN_TL2] = BTN_SELECT;
    buttonMap[BTN_TR2] = BTN_START;
    buttonMap[BTN_MODE] = BTN_MODE;
    axisMap[ABS_Z] = ABS_RX;
    axisMap[BTN_Z] = ABS_RY;
    axisMap[ABS_HAT0X] = ABS_HAT0X;
    axisMap[ABS_HAT0Y] = ABS_HAT0Y;
}