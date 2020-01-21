/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ActiveObject.h
 * Author: Francesco Alongi
 *
 * Created on 21 gennaio 2020, 10.15
 */

#ifndef ACTIVEOBJECT_H
#define ACTIVEOBJECT_H

#include <atomic>
#include <miosix.h>

class ActiveObject {
public:
    ActiveObject();
    virtual ~ActiveObject();
private:
    virtual void run();
    ActiveObject& operator=(const ActiveObject& ao) = delete;
    ActiveObject(const ActiveObject& orig) = delete;
protected:
    miosix::Thread t;
    std::atomic<bool> quit;
};

#endif /* ACTIVEOBJECT_H */

