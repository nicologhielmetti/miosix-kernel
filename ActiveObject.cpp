/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ActiveObject.h"
#include <cstdio>
#include <functional>


using namespace miosix;

ActiveObject::ActiveObject(): quit(false) {}

void ActiveObject::threadLauncher(void* argv)
{
    printf("AO threadLauncher\n");
    reinterpret_cast<ActiveObject*>(argv)->run();
}

void ActiveObject::start()
{
    //t = Thread::create(threadLauncher, 2048, Priority(), this, Thread::JOINABLE);
}

void ActiveObject::run(){
    while(!quit.load())
    {
        
    }
}

ActiveObject::~ActiveObject() {
    printf("AO destr\n");
    if(quit.load()) return;
    quit.store(true);
    t->join();
}