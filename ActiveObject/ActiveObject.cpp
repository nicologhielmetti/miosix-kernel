/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ActiveObject.h"

#include<functional>

using namespace std;

ActiveObject::ActiveObject(): quit(false), t(miosix::Thread.create(&ActiveObject::run,this)) {}

void ActiveObject::run(){
    while(quit.load())
    {
        
    }
}

ActiveObject::~ActiveObject() {
    if(quit.load()) return;
    quit.store(true);
    t.join();
}