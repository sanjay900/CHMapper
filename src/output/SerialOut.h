//
// Created by sanjay on 15/07/18.
//

#ifndef SANPIE_SERIALOUT_H
#define SANPIE_SERIALOUT_H


#include <src/sol.hpp>
#include <src/Serial.hpp>
#include "Output.hpp"

class SerialOut : public Serial, public Output {
public:
    SerialOut(const std::string &lua_name, sol::table &lua_table);

private:
    bool try_to_use_device(struct udev *udev, struct udev_device *device, sol::state &lua) override;

    bool try_disconnect(const std::string &sysname, sol::state *lua) override;
};


#endif //SANPIE_SERIALOUT_H
