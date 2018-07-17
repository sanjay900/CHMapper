//
// Created by sanjay on 18/07/18.
//

#ifndef SANPIE_OUTPUT_H
#define SANPIE_OUTPUT_H


#include <src/sol.hpp>
#include <src/Device.hpp>

class Output : public Device {
public:
    static Output* create(std::string &name, sol::table &lua_dev, sol::state &lua);
};


#endif //SANPIE_OUTPUT_H
