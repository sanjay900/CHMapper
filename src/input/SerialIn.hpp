//
// Created by sanjay on 14/07/18.
//

#ifndef SANPIE_SERIALIN_H
#define SANPIE_SERIALIN_H


#include <termios.h>
#include <src/sol.hpp>
#include <src/Serial.hpp>
#include "Input.hpp"

class SerialIn: public virtual Input, public Serial {
    bool try_to_use_device(struct udev*, struct udev_device*, sol::state &lua) override;
    bool try_disconnect(const std::string &sysname,sol::state *lua) override;
    void tick(sol::state &lua) override;
public:
    bool isValid() const override;
    SerialIn(const std::string &lua_name, sol::table &dev);
};


#endif //SANPIE_SERIAL_H
