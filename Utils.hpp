//
// Created by sanjay on 13/07/18.
//

#ifndef SANPIE_UTILS_H
#define SANPIE_UTILS_H


#include <string>
#include "sol.hpp"

class Utils {
public:
    static int scale(int x, int in_min, int in_max, int out_min, int out_max);
    static bool string_starts_with(std::string string, std::string substring);
    static bool string_ends_with(std::string string, std::string substring);
    static void register_utils(sol::state &lua);
};


#endif //SANPIE_UTILS_H
