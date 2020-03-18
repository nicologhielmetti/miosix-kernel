//default baudrate 19200
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectroni
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */


#include <cstdio>
#include <kernel/kernel.h>
#include "Lps22hb.h"
#include "SyncQueue.h"
#include "NeuralNetwork.h"

using namespace std;
using namespace miosix;

#ifdef MAIN_PROFILING
#define MAIN 1
#else
#define MAIN 0
#endif

//pressure sensor address
const unsigned char lps22hb_addr = 0xBA;

// TO BE CHECKED BEFORE EACH COMPILATION
// altitude of the sensor (in meters), used to compute the sea-level atm pressure
const unsigned int sensor_altitude = 130;

void initRCC(){
    //enable RCC pheripherals
    FastInterruptDisableLock a;
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    CRC->CR = CRC_CR_RESET;
}

typedef Gpio<GPIOB_BASE,9>  sda;
typedef Gpio<GPIOB_BASE,8>  scl;
Lps22hb<sda,scl,lps22hb_addr> pressure_sensor(sensor_altitude); 
SyncQueue<float> in_queue;

int main()
{   
    initRCC();
    //initialize the sensor via I2C
    if(MAIN) MemoryProfiling::print("THREAD_MAIN,ps.init()");
    pressure_sensor.init();
    //it starts the neural network which is an active object
    if(MAIN) MemoryProfiling::print("THREAD_MAIN,nn()");
    NeuralNetwork nn(in_queue, pressure_sensor.getODR());
    for(;;)
    {
        //This call block the main thread until PB10 pass from 0 to 1.
        //When it happens it means that the fifo is full and it can be read 
        if(MAIN) MemoryProfiling::print("THREAD_MAIN,ps.waitForFullFifo()");
        pressure_sensor.waitForFullFifo();
        //This function read the 32 slots of the fifo, calculate the avg and 
        //return the value reshaped considering the altitude of the measure
        if(MAIN) MemoryProfiling::print("THREAD_MAIN,ps.getLast32AvgPressure()");
        float pressure_val = pressure_sensor.getLast32AvgPressure();
        //printf("Pressure reading: %f \n", pressure_val);
        if(MAIN) MemoryProfiling::print("THREAD_MAIN,ps.in_queue.put()");
        in_queue.put(pressure_val);
        if(MAIN) MemoryProfiling::print("\0");
    }
 }