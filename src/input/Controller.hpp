//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_CONTROLLER_H
#define WEJOY_CONTROLLER_H


#include <map>
#include "src/sol.hpp"
#include "Input.hpp"

class Controller : public Input {
    struct libevdev *dev = nullptr;
    std::vector<int> buttonBindings;
    std::map<int,int> buttonTypeBindings;
    std::vector<int> axisBindings;
    std::map<int,int> axisTypeBindings;
    int fd;
    int get_axis_min(uint type);

    int get_axis_max(uint type);
public:
    Controller(const std::string&, const std::string&, sol::table&);
    static int scale(int x, int in_min, int in_max, int out_min, int out_max);

    bool try_to_use_device(struct udev*, struct udev_device*, sol::state &lua) override;

    bool try_disconnect(const std::string &sysname,sol::state *lua) override;

    bool isValid() const override;

    void initMaps();

    void tick(sol::state &lua) override;

    virtual ~Controller();
};


#endif
