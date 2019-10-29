#include "PS3.h"
#include <iostream>

PS3::PS3(GtkTextBuffer *buffer, std::string name, std::string dev, struct libevdev *_dev, PS3Type type): Input(buffer, name, dev, _dev), type(type) {

}
void PS3::init()
{
    //Slider is ABS_RZ
    Input::init();
    buttonMap[BTN_EAST] = BTN_SOUTH;
    buttonMap[BTN_C] = BTN_EAST;
    if (type == PS3Type::RB) {
        buttonMap[BTN_NORTH] = BTN_WEST;
        buttonMap[BTN_SOUTH] = BTN_NORTH;
    } else {
        buttonMap[BTN_NORTH] = BTN_NORTH;
        buttonMap[BTN_SOUTH] = BTN_WEST;
    }
    

    buttonMap[BTN_WEST] = BTN_TL;
    buttonMap[BTN_MODE] = BTN_MODE;
    axisMap[ABS_Z] = ABS_RX;
    axisMapScaled[ABS_Z] = [](int val) { return (-32767+(val*2)); };
    if (type == PS3Type::GH) {
        buttonMap[BTN_SELECT] = BTN_SELECT;
        buttonMap[BTN_START] = BTN_START;
        axisMap[ABS_RY] = ABS_RY;
        axisMapScaled[ABS_RY] = [](int val) { return (val * -2) - 28000; };
    }
    if (type == PS3Type::RB) {
        buttonMap[BTN_TL2] = BTN_SELECT;
        buttonMap[BTN_TR2] = BTN_START;
        axisMap[BTN_Z] = ABS_RY;
    }
    axisMap[ABS_HAT0X] = ABS_HAT0X;
    axisMap[ABS_HAT0Y] = ABS_HAT0Y;
}