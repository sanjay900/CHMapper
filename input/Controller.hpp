//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_CONTROLLER_H
#define WEJOY_CONTROLLER_H


#include <map>
#include "sol.hpp"
#include "Input.hpp"

class Controller : public Input {
    friend class InputFactory;
    friend class Wiimote;
    struct libevdev *dev = nullptr;
    std::vector<int> buttonBindings;
    std::map<int,int> buttonTypeBindings;
    std::vector<int> axisBindings;
    std::map<int,int> axisTypeBindings;

    int get_axis_min(uint type);

    int get_axis_max(uint type);
protected:
    Controller(const std::string&, const std::string&, sol::table&);
public:
    static int scale(int x, int in_min, int in_max, int out_min, int out_max);
    virtual bool try_to_use_device(struct udev*, struct udev_device*, sol::state &lua);
    virtual bool try_disconnect(const std::string &sysname,sol::state *lua);

    virtual bool isValid() const;

    void initMaps();
    virtual void tick(sol::state &lua);

    virtual ~Controller();
};


#endif
