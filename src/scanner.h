#pragma once
#include "input/Input.h"
#include <list>
#include <gtk/gtk.h>
class Scanner
{
public:
    Scanner(GtkTextBuffer *buffer);
    void scan(std::list<Input*> *inputs);
    void findNew(std::list<Input*> *inputs);
    void construct(udev_device *device, std::list<Input*> *inputs);

private:
    GtkTextBuffer *buffer;
    struct udev_monitor *mon;
    int fd;
    fd_set fds;
    struct timeval tm;
    struct udev *udev;
};