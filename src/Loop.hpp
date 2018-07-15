//
// Created by sanjay on 7/07/18.
//

#ifndef SANPIE_TICKTHREAD_H
#define SANPIE_TICKTHREAD_H


#include <thread>
#include "sol.hpp"

class Loop {
private:

    void tick();
    bool running = true;
    sol::state& lua;
    std::thread tickThread;
public:
    explicit Loop(sol::state &lua): lua(lua),tickThread(&Loop::tick,this){}
    void stop();
};


#endif //SANPIE_TICKTHREAD_H
