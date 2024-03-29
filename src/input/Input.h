#pragma once
#include <libudev.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <libevdev-1.0/libevdev/libevdev-uinput.h>
#include <string>
#include <map>
#include <functional>
#include <gtk/gtk.h>
class Input
{
public:
    Input(GtkTextBuffer *buffer, std::string name, std::string dev, struct libevdev *_dev);
    //used by wii extensions
    Input(GtkTextBuffer *buffer, std::string name, std::string dev, struct libevdev *_dev, struct libevdev_uinput *uidev); 
    GtkTextBuffer *buffer;
    std::string dev;
    std::string name;
    bool child;
    //Input evdev device
    struct libevdev *_dev;
    //uinput device
    struct libevdev_uinput *uidev;
    //Map from an axis on the original controller to one on the new controller
    std::map<int,int> axisMap;
    //Apply a function to the original axis before applying to the new controller
    std::map<int,std::function<int(int)>> axisMapScaled;
    //Map a button from the original controller to one on the new controller
    std::map<int,int> buttonMap;
    virtual bool has_children();
    void disconnect();
    void tick();
    virtual void init();
    virtual void add_child(Input* input);
};