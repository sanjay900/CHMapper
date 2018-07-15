//
// Created by sanjay on 15/07/18.
//

#ifndef SANPIE_SERIAL_H
#define SANPIE_SERIAL_H


#include <termios.h>
#include "sol.hpp"

class Serial {
    struct termios oldtio, newtio;
    speed_t baudrate;

protected:
    bool isValid() const;
    int fd = -1;
public:
    Serial(sol::table &dev);
    ~Serial();
};


#endif //SANPIE_SERIAL_H
