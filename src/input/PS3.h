#pragma once
#include "Input.h"
enum PS3Type {
    Controller,
    GH,
    RB
};
class PS3: public Input {
public:
    PS3(GtkTextBuffer *buffer, std::string name, std::string dev, struct libevdev *_dev, PS3Type type);
    void init();
    PS3Type type;
};