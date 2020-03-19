//default baudrate 19200
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include <iostream>
#include <kernel/kernel.h>
#include "miosix.h"
#include "Lps22hb.h"
#include "SyncQueue.h"
#include "NeuralNetwork.h"
#include "util/util.h"
#include "profiling_defines.h"

using namespace std;
using namespace miosix;

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
#ifdef MAIN_PROFILING_TIMING
    tBefore = getTick();
    pressure_sensor.init();
    printf("ps.init() timing : %lld \n", (getTick() - tBefore));
#else
    pressure_sensor.init();
#endif

    //it starts the neural network which is an active object
    NeuralNetwork nn(in_queue, pressure_sensor.getODR());
    for(;;)
    {
        //This call block the main thread until PB10 pass from 0 to 1.
        //When it happens it means that the fifo is full and it can be read
#ifdef MAIN_PROFILING_TIMING
        tBefore = getTick();
        pressure_sensor.waitForFullFifo();
        printf("ps.waitForFullFifo() timing : %lld \n", (getTick() - tBefore));
#else
        pressure_sensor.waitForFullFifo();
#endif
        //This function read the 32 slots of the fifo, calculate the avg and 
        //return the value reshaped considering the altitude of the measure
#ifdef MAIN_PROFILING_TIMING
        tBefore = getTick();
        float pressure_val = pressure_sensor.getLast32AvgPressure();
        printf("ps.getLast32AvgPressure() timing : %lld \n", (getTick() - tBefore));
#else
        float pressure_val = pressure_sensor.getLast32AvgPressure();
#endif
        //printf("Pressure reading: %f \n", pressure_val);
#ifdef MAIN_PROFILING_TIMING
        tBefore = getTick();
        in_queue.put(pressure_val);
        printf("in_queue.put() timing : %lld \n", (getTick() - tBefore));
#else
        in_queue.put(pressure_val);
#endif
    }
 }