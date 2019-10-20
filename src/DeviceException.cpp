#include "DeviceException.h"

DeviceException::DeviceException(std::string message):message_(std::move(message)){}