//
// Created by sanjay on 13/07/18.
//

#ifndef SANPIE_OUTPUTFACTORY_H
#define SANPIE_OUTPUTFACTORY_H


#include <string>
#include <sol.hpp>

class OutputFactory {
    static void create(std::string &name, sol::table &lua_dev, sol::state &lua);
};


#endif //SANPIE_OUTPUTFACTORY_H
