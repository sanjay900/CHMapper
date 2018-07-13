//
// Created by sanjay on 13/07/18.
//

#ifndef SANPIE_INPUTFACTORY_H
#define SANPIE_INPUTFACTORY_H


#include <ControllerException.hpp>
#include "Controller.hpp"
#include "Wiimote.hpp"
#include "MIDI.hpp"
#include "SER_MIDI.hpp"

class InputFactory {
    static Controller* create(std::string &name, sol::table &lua_table);
};


#endif //SANPIE_INPUTFACTORY_H
