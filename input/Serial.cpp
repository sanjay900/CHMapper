//
// Created by sanjay on 14/07/18.
//

#include <DeviceException.hpp>
#include <fcntl.h>
#include <linux/serial.h>
#include <termio.h>
#include <zconf.h>
#include "Serial.hpp"

bool Serial::try_to_use_device(struct udev *, struct udev_device *, sol::state &lua) {
    return false;
}

bool Serial::try_disconnect(const std::string &sysname, sol::state *lua) {
    return false;
}

void Serial::tick(sol::state &lua) {
    unsigned char buf;
    if (read(fd, &buf, 1) < 0) {
        return;
    }
    auto func = lua["serial_in"];
    if (func.valid()) {
        func(lua_table, buf);
    }
}

Serial::~Serial() {
    if(isValid()) {
        tcsetattr(fd, TCSANOW, &oldtio);
        close(fd);
        fd = -1;
    }
}

Serial::Serial(const std::string &lua_name, sol::table &dev) : Input(lua_name, "Serial", dev) {
    switch (lua_table.get_or("baudrate",115200)) {
        case 1200   :baudrate = B1200  ; break;
        case 2400   : baudrate = B2400  ; break;
        case 4800   : baudrate = B4800  ; break;
        case 9600   : baudrate = B9600  ; break;
        case 19200  : baudrate = B19200 ; break;
        case 38400  : baudrate = B38400 ; break;
        case 57600  : baudrate = B57600 ; break;
        case 115200 : baudrate = B115200; break;
        default     : throw DeviceException("Unknown baudrate");
    }
    sysname = lua_table["device"];
    lua_table["name"] = lua_name;
    fd = open(sysname.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) {
        throw DeviceException("Unable to open serial port "+sysname);
    }
    struct serial_struct ser_info{};
    /* save current serial port settings */
    tcgetattr(fd, &oldtio);

    /* clear struct for new port settings */
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = baudrate | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;
    newtio.c_cc[VTIME]    = 0;
    newtio.c_cc[VMIN]     = 1;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);
    ioctl(fd, TIOCGSERIAL, &ser_info);
    ser_info.flags |= ASYNC_LOW_LATENCY;
    ioctl(fd, TIOCSSERIAL, &ser_info);
    std::cout << "Waiting for midi control signal from " << lua_name << std::endl;
}
