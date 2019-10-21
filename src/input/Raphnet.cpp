#include "Raphnet.h"
#include <iostream>
void Raphnet::init()
{
    Input::init();
    //Touchbar is ABS_RY, but we can ignore that.
    buttonMap[BTN_WEST] = BTN_SOUTH;
    buttonMap[BTN_EAST] = BTN_EAST;
    buttonMap[BTN_Z] = BTN_WEST;
    buttonMap[BTN_SOUTH] = BTN_NORTH;
    buttonMap[BTN_TL2] = BTN_TL;
    buttonMap[BTN_C] = BTN_SELECT;
    buttonMap[BTN_NORTH] = BTN_START;
    axisMap[ABS_Z] = ABS_RX;
    axisMap[ABS_X] = ABS_HAT0X;
    axisMap[ABS_Y] = ABS_HAT0Y;
    axisMap[BTN_THUMBL] = -ABS_HAT0Y;
    axisMap[BTN_MODE] = ABS_HAT0Y;
}