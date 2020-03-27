/* 
 * File:   SyncQueue.h
 * Author: nicolo
 *
 * Created on January 21, 2020, 1:58 AM
 */

#ifndef SYNCQUEUE_H
#define	SYNCQUEUE_H

#include "miosix/kernel/sync.h"
#include "miosix/util/util.h"
#include "profile_defines.h"
#include <cstdio>
#include <list>

using namespace miosix;

template<typename T>
class SyncQueue {
public:    
    SyncQueue()
    {
#ifdef MAIN_PROFILING 
        usedStackEnd = MemoryProfiling::getCurrentUsedStack();
#endif
    }
    
    void put(const T& data) 
    {
        Lock<Mutex> lck(myMutex);
        queue.push_back(data);
        myCv.signal();
        #ifdef MAIN_PROFILING  
            usedStackEnd = MemoryProfiling::getCurrentUsedStack();
        #endif
    }
    
    T get() 
    {
        Lock<Mutex> lck(myMutex);
        while(queue.empty())
            myCv.wait(lck);
        T result=queue.front();
        queue.pop_front();
        #ifndef MAIN_PROFILING  
            //stack.put(MemoryProfiling::getCurrentUsedStack());
        usedStackEnd = MemoryProfiling::getCurrentUsedStack();
        #endif
        return result;
    }
    
    size_t size() 
    {
        Lock<Mutex> lck(myMutex);
        return queue.size();
    }
    
protected:
    std::list<T> queue;
    Mutex myMutex;
    ConditionVariable myCv;
    
private:
    SyncQueue(const SyncQueue &)=delete;
    SyncQueue & operator=(const SyncQueue &)=delete;
};

#endif	/* SYNCQUEUE_H */

