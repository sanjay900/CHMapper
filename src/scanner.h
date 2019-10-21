#pragma once
#include "input/Input.h"
#include <list>
class Scanner
{
public:
    Scanner();
    void scan(std::list<Input*> *inputs);
    void findNew(std::list<Input*> *inputs);
    void construct(udev_device *device, std::list<Input*> *inputs);

private:
    struct udev_monitor *mon;
    int fd;
    fd_set fds;
    struct timeval tm;
    struct udev *udev;
};