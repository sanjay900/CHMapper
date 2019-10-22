#include <iostream>
#include <libudev.h>
#include <chrono>
#include <thread>
#include <fcntl.h>
#include <zconf.h>
#include "scanner.h"
#include "input/Input.h"
#include <list>
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "Started CHMapper, waiting for Wii Guitar, PS3 Guitar or Raphnet Guitar" << std::endl;
    std::list<Input *> inputs;
    Scanner *scanner = new Scanner();
    scanner->scan(&inputs);
    //Scan twice, once to pick up wiimotes, and once to pick up their extensions.
    scanner->scan(&inputs);
    while (true) {
        scanner->findNew(&inputs);
        for (auto x: inputs) {
            x->tick();
        }
    }
    return 0;
}