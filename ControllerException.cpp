//
// Created by sanjay on 4/07/18.
//

#include <string>
#include "ControllerException.h"

ControllerException::ControllerException(std::string message):message_(std::move(message)){}