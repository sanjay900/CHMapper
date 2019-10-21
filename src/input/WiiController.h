#pragma once
#include "Input.h"
class WiiController: public Input {
    Input* accel = nullptr;
    Input* extension;
    bool isGuitar;
    using Input::Input;
    void add_child(Input* input);
};