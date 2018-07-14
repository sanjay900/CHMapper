//
// Created by sanjay on 15/07/18.
//

#ifndef SANPIE_SERIALOUT_H
#define SANPIE_SERIALOUT_H


#include <sol.hpp>
#include <Serial.hpp>

class SerialOut : Serial {
public:
    SerialOut(const std::string &lua_name, sol::table &lua_table);
};


#endif //SANPIE_SERIALOUT_H
