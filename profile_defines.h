/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   profile_defines.h
 * Author: nicolo
 *
 * Created on March 18, 2020, 10:47 PM
 */

#ifndef PROFILE_DEFINES_H
#define PROFILE_DEFINES_H

#include "ProfileSynchQueue.h"

#ifdef __cplusplus
extern "C" {
#endif
    
//static ProfileSyncQueue<int> stack;    
extern int usedStackBefore, usedStackEnd;
    
#define MAIN_PROFILING


#ifdef __cplusplus
}
#endif

#endif /* PROFILE_DEFINES_H */

