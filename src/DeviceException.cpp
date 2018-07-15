//
// Created by sanjay on 4/07/18.
//

#include <string>
#include "DeviceException.hpp"

DeviceException::DeviceException(std::string message):message_(std::move(message)){}