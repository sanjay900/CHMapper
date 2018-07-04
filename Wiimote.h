//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_WIIMOTE_H
#define WEJOY_WIIMOTE_H


#include "Controller.hpp"

class Wiimote: Controller {
    friend class Controller;
    std::string extension;
    Wiimote(const std::string &name, const std::string&, sol::table &dev);

};


#endif
