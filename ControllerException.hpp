//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_CONTROLLEREXCEPTION_H
#define WEJOY_CONTROLLEREXCEPTION_H


#include <exception>
#include <string>
#include <utility>

class ControllerException: public std::exception {
private:
    const std::string message_;
public:
    explicit ControllerException(std::string message);
    virtual const char* what() const noexcept {
        return message_.c_str();
    }
};
#endif
