/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ActiveObject.h"

#include<functional>


using namespace miosix;

ActiveObject::ActiveObject(): t(Thread::create(threadLauncher, 2048, Priority(), this, Thread::JOINABLE)), quit(false) {}

void ActiveObject::threadLauncher(void* argv)
{
    reinterpret_cast<ActiveObject*>(argv)->run();
}

void ActiveObject::run()
{
    while(!quit.load())
    {
        
    }
}

ActiveObject::~ActiveObject()
{
    if(quit.load()) return;
    quit.store(true);
    t->join();
}