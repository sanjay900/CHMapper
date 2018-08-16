//
// Created by sanjay on 15/07/18.
//

#ifndef SANPIE_SERIAL_H
#define SANPIE_SERIAL_H


#include <termios.h>
#include "sol.hpp"
#include "Device.hpp"

class Serial : virtual public Device {
    struct termios oldtio, newtio;
    speed_t baudrate;

protected:
    bool isValid() const;
    int fd = -1;
    std::string sysname;
public:
    bool try_to_use_device(struct udev *udev, struct udev_device *device, sol::state &lua) override;

    bool try_disconnect(const std::string &sysname, sol::state *lua) override;

    Serial(sol::table &dev);
    ~Serial();
};


#endif //SANPIE_SERIAL_H
