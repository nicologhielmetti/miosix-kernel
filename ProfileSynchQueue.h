/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProfileSynchQueue.h
 * Author: nicolo
 *
 * Created on March 27, 2020, 1:00 AM
 */

#ifndef PROFILESYNCHQUEUE_H
#define PROFILESYNCHQUEUE_H
#include <vector>
#include <list>
#include "miosix/kernel/sync.h"


using namespace miosix;

template<typename T>
class ProfileSyncQueue {
public:    
    ProfileSyncQueue()
    {}
    
    void put(const T& data) 
    {
        Lock<Mutex> lck(myMutex);
        queue.push_back(data);
        if(queue.size() > 1)
            myCv.signal();
        printf("size after put -> %u\n", queue.size());
    }
    
    std::vector<T> get() 
    {   
        Lock<Mutex> lck(myMutex);
        printf("size before get -> %u\n",queue.size());
        while(queue.size() < 2)
            myCv.wait(lck);
        std::vector<T> l;
        l.push_back(queue.back());
        l.push_back(queue.back());
        queue.pop_back();
        queue.pop_back();
        printf("size end get -> %u\n",queue.size());
        return l;
    }
    
    size_t _size() 
    {
        Lock<Mutex> lck(myMutex);
        return queue.size();
    }
    
protected:
    std::list<T> queue;
    Mutex myMutex;
    ConditionVariable myCv;
    
private:
    ProfileSyncQueue(const ProfileSyncQueue &)=delete;
    ProfileSyncQueue & operator=(const ProfileSyncQueue &)=delete;
};

#endif /* PROFILESYNCHQUEUE_H */

