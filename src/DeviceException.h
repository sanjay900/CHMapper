#pragma once


#include <exception>
#include <string>
#include <utility>

class DeviceException: public std::exception {
private:
    const std::string message_;
public:
    explicit DeviceException(std::string message);
    virtual const char* what() const noexcept {
        return message_.c_str();
    }
};
