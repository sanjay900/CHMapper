//
// Created by sanjay on 14/07/18.
//

#ifndef SANPIE_SERIAL_H
#define SANPIE_SERIAL_H


#include <termios.h>
#include <sol.hpp>
#include "Controller.hpp"

class Serial: public virtual Controller {
    friend class MIDISerial;
    friend class InputFactory;
    Serial(const std::string &lua_name, sol::table &dev);
    struct termios oldtio, newtio;
    speed_t baudrate;
    bool try_to_use_device(struct udev*, struct udev_device*, sol::state &lua) override;
    bool try_disconnect(const std::string &sysname,sol::state *lua) override;
    void tick(sol::state &lua) override;
    ~Serial() override;
};


#endif //SANPIE_SERIAL_H
