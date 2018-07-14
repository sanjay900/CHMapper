//
// Created by sanjay on 14/07/18.
//

#ifndef SANPIE_SERIAL_H
#define SANPIE_SERIAL_H


#include <termios.h>
#include <sol.hpp>
#include "Input.hpp"

class Serial: public virtual Input {
    struct termios oldtio, newtio;
    speed_t baudrate;
    bool try_to_use_device(struct udev*, struct udev_device*, sol::state &lua) override;
    bool try_disconnect(const std::string &sysname,sol::state *lua) override;
    void tick(sol::state &lua) override;
public:
    Serial(const std::string &lua_name, sol::table &dev);
    ~Serial();
};


#endif //SANPIE_SERIAL_H
