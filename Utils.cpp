//
// Created by sanjay on 13/07/18.
//

#include "Utils.hpp"

int Utils::scale(int x, int in_min, int in_max, int out_min, int out_max) {
    return 0;
}

bool Utils::string_starts_with(std::string string, std::string substring) {
    return string.size() >= substring.size()
           && equal(substring.begin(), substring.end(), string.begin());;
}

bool Utils::string_ends_with(std::string string, std::string substring) {
    return string.size() >= substring.size()
           && equal(substring.rbegin(), substring.rend(), string.rbegin());;
}

void Utils::register_utils(sol::state &lua) {
    sol::table math = lua["math"];
    math["scale"]= scale;
    sol::table string = lua["string"];
    string["starts"] = string_starts_with;
    string["ends"] = string_ends_with;
}
