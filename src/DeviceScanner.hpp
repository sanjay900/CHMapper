//
// Created by sanjay on 13/07/18.
//

#ifndef SANPIE_DEVICESCANNER_H
#define SANPIE_DEVICESCANNER_H


#include <src/input/Input.hpp>
#include <src/output/Output.hpp>
#include "sol.hpp"

class DeviceScanner {
    std::vector<Device*> devices;
    void scan_devices(sol::state &lua);
    void scan_v_devices(sol::state &lua);
    void scan_connected_devices(sol::state &lua);
public:
    explicit DeviceScanner(sol::state &lua);
};


#endif //SANPIE_DEVICESCANNER_H
