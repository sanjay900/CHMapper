//
// Created by sanjay on 13/07/18.
//

#ifndef SANPIE_INPUTFACTORY_H
#define SANPIE_INPUTFACTORY_H


#include <DeviceException.hpp>
#include "Input.hpp"
#include "Wiimote.hpp"

class InputFactory {
public:
    static Input* create(std::string &name, sol::table &lua_table);
};


#endif //SANPIE_INPUTFACTORY_H
