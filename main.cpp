#include <iostream>
#include <libudev.h>
#include "sol.hpp"
#include "input/Input.hpp"
#include "DeviceException.hpp"
#include <chrono>
#include <thread>
#include <Loop.hpp>
#include <fcntl.h>
#include <zconf.h>
#include "Utils.hpp"
#include "DeviceScanner.hpp"

int main(int argc, char *argv[]) {
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::io, sol::lib::math);
    if (argc < 0) {
        std::cerr << "A script is required as the first argument" << std::endl;
    }
    lua.script_file(argv[1]);
    Utils::register_utils(lua);
    new DeviceScanner(lua);
    Loop loop = Loop(lua);
    std::cout << "Press 'q' and then 'ENTER' to quit!\n";
    while (getchar() != 'q');
    loop.stop();
    sleep(1);
    return 0;
}