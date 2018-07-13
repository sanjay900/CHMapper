//
// Created by sanjay on 13/07/18.
//

#ifndef SANPIE_INPUTFACTORY_H
#define SANPIE_INPUTFACTORY_H


#include <ControllerException.hpp>
#include "Controller.hpp"
#include "Wiimote.hpp"
#include "MIDIDirect.hpp"
#include "MIDISerial.hpp"

class InputFactory {
public:
    static Controller* create(std::string &name, sol::table &lua_table);
};


#endif //SANPIE_INPUTFACTORY_H
