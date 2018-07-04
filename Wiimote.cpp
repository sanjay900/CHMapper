//
// Created by sanjay on 4/07/18.
//

#include "Wiimote.h"
#include "ControllerException.h"

Wiimote::Wiimote(const std::string &name, const std::string &dev_name, sol::table &dev): Controller(name, dev_name, dev) {

}
