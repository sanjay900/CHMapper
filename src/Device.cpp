//
// Created by sanjay on 18/07/18.
//

#include "Device.hpp"


bool operator<(const Device &lhs, const Device &rhs) {
    return lhs.lua_name < rhs.lua_name;
}

bool operator>(const Device &lhs, const Device &rhs) {
    return rhs < lhs;
}

bool operator<=(const Device &lhs, const Device &rhs) {
    return !(rhs < lhs);
}

bool operator>=(const Device &lhs, const Device &rhs) {
    return !(lhs < rhs);
}