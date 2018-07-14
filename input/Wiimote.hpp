//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_WIIMOTE_H
#define WEJOY_WIIMOTE_H


#include "Controller.hpp"

class Wiimote: Controller {
    friend class InputFactory;
    std::string extension_name;
    Input* extension;
    Input* ir;
    Input* accelerometer;
    Input* motion_plus;
    Wiimote(const std::string &name, sol::table &dev);
public:
    bool try_to_use_device(struct udev*, struct udev_device*, sol::state &lua) override;
    bool try_disconnect(const std::string &sysname,sol::state *lua) override;
    void tick(sol::state &lua) override;
    ~Wiimote() override;

};


#endif
