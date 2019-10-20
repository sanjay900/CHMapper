#pragma once
#include "Input.h"
class WiiController: public Input {
    Input* accel;
    Input* extension;
    using Input::Input;
    void add_child(Input* input);
};