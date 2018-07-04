//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_CONTROLLER_H
#define WEJOY_CONTROLLER_H


#include <map>
#include "sol.hpp"

class Controller {
    friend class Wiimote;
    sol::table lua_table;
    std::string lua_name;
    std::string name;
    struct libevdev *dev = nullptr;
    int fd = -1;
    std::vector<int> buttonBindings;
    std::map<int,int> buttonTypeBindings;
    std::vector<int> axisBindings;
    std::map<int,int> axisTypeBindings;
protected:
    Controller(const std::string&, const std::string&, sol::table&);
public:
    static Controller* create(std::string&,sol::table&);

    virtual bool try_to_use_device(struct udev*, struct udev_device*);
    void disconnect();
    const std::string &getLua_name() const;

    const std::string &getName() const;

    bool isValid() const;

    void initMaps();
};


#endif
