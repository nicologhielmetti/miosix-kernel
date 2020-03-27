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
//#include "profile_defines.h"
#include "ActiveObject.h"

using namespace std;
using namespace miosix;

int usedStackBefore, usedStackEnd;

#ifndef MAIN_PROFILING
static SyncQueue<float> queue;
static unsigned int acquiredValues = 0;
static ai_float incrementalMean = 0;
static OdrMode odr;
static ai_float in_data[AI_NETWORK_IN_1_SIZE] = {0, 0, 0};
static ai_handle network = AI_HANDLE_NULL;
static ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
static ai_buffer nn_input[AI_NETWORK_IN_NUM] =  AI_NETWORK_IN;
static ai_buffer nn_output[AI_NETWORK_OUT_NUM] =  AI_NETWORK_OUT;
static ai_float nn_outdata[AI_NETWORK_OUT_1_SIZE];

static const float normMin = 978.52708333;
static const float normMax = 1040.8893617;
#endif
void initRCC(){
    //enable RCC pheripherals
    FastInterruptDisableLock a;
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    CRC->CR = CRC_CR_RESET;
}

typedef Gpio<GPIOB_BASE,9>  sda;
typedef Gpio<GPIOB_BASE,8>  scl;

#ifdef MAIN_PROFILING
void _main(void*)
{
    
    //pressure sensor address
    const unsigned char lps22hb_addr = 0xBA;

    // TO BE CHECKED BEFORE EACH COMPILATION
    // altitude of the sensor (in meters), used to compute the sea-level atm pressure
    const unsigned int sensor_altitude = 130;
 
    usedStackBefore = MemoryProfiling::getCurrentUsedStack();
    Lps22hb<sda,scl,lps22hb_addr> pressure_sensor(sensor_altitude); 
    printf("--- DELTA ps constructor: %i ---\n", usedStackEnd-usedStackBefore);
    
    usedStackBefore = MemoryProfiling::getCurrentUsedStack();
    SyncQueue<float> queue;
    printf("--- DELTA queue constructor: %i ---\n", usedStackEnd-usedStackBefore);
    
    usedStackBefore = MemoryProfiling::getCurrentUsedStack();          
    pressure_sensor.init();
    printf("--- DELTA ps.init(): %i ---\n", usedStackEnd-usedStackBefore);

    for(;;)
    { 

        usedStackBefore = MemoryProfiling::getCurrentUsedStack();
        pressure_sensor.waitForFullFifo();
        printf("--- DELTA ps.waitForFullFifo(): %i ---\n", usedStackEnd-usedStackBefore);
 
        usedStackBefore = MemoryProfiling::getCurrentUsedStack();      
        float pressure_val = pressure_sensor.getLast32AvgPressure();
        printf("--- DELTA ps.getLast32AvgPressure(): %i ---\n", usedStackEnd-usedStackBefore);

        usedStackBefore = MemoryProfiling::getCurrentUsedStack();
        queue.put(pressure_val);
        printf("--- DELTA queue.put(): %i ---\n", usedStackEnd-usedStackBefore);

        //consume produced value
        Thread::sleep(1000);
        queue.get();
    }

}
#endif

#ifndef MAIN_PROFILING
void initNN()
{
    ai_error err = ai_network_create(&network, (const ai_buffer*)AI_NETWORK_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE) 
    {
        printf("E: AI error in NN creation - type=%lu code=%lu\r\n", err.type, err.code);
        // TODO: error handling
    }
    else 
    {
        //printf("NN successfully created.\n");
    }

    // network initialization
    const ai_network_params params = {
    AI_NETWORK_DATA_WEIGHTS(ai_network_data_weights_get()),
    AI_NETWORK_DATA_ACTIVATIONS(activations) };

    if (!ai_network_init(network, &params)) 
    {
        err = ai_network_get_error(network);
        printf("E: AI error in NN initialization - type=%lu code=%lu\r\n", err.type, err.code);
        // TODO: error handling
    } 
    else 
    {
        //printf("NN successfully initialized.\n");
    }
    //stack.put(MemoryProfiling::getCurrentUsedStack());
    usedStackEnd = MemoryProfiling::getCurrentUsedStack();
}
#endif

#ifndef MAIN_PROFILING
int  runNN(ai_handle network, void* input)
{
    ai_i32 nbatch;
    ai_error err;

    nn_input[0].n_batches = 1;
    nn_input[0].data = AI_HANDLE_PTR(input);
    nn_output[0].n_batches = 1;
    nn_output[0].data = AI_HANDLE_PTR(nn_outdata);

    nbatch = ai_network_run(network, &nn_input[0], &nn_output[0]);
    if (nbatch != 1) 
    {
        err = ai_network_get_error(network);
    }
    //stack.put(MemoryProfiling::getCurrentUsedStack());
    usedStackEnd = MemoryProfiling::getCurrentUsedStack();
    return 1;    
}
#endif

#ifndef MAIN_PROFILING
void enqueue (ai_float* input, ai_float newValue)
{
    for (int i=0; i < AI_NETWORK_IN_1_SIZE-1; i++) 
    {
        input[i] = input[i+1];
    }
    input[AI_NETWORK_IN_1_SIZE - 1] = newValue; 
}
#endif

#ifndef MAIN_PROFILING
ai_float*  normalizeInput(ai_float* input)
{
    for (int i=0; i < AI_NETWORK_IN_1_SIZE; i++) 
    {
        // if the value is between the pressure range of the sensor, it must be
        // normalized.
        if (input[i] > 260 && input[i] < 1260)
            input[i] = (input[i] - normMin) / (normMax - normMin);
        //printf("element number %d of the queue before prediction: %f\n",i+1,input[i]);
    }
    return input;
}
#endif


#ifndef MAIN_PROFILING
void nn(void*)
{
    //stack.put(MemoryProfiling::getCurrentUsedStack());
    usedStackBefore = MemoryProfiling::getCurrentUsedStack();
    NeuralNetwork nn(queue, OdrMode::ODR_1HZ);
    //std::vector<int> l = stack.get();
    printf("--- DELTA nn constuctor: %i ---\n", usedStackEnd - usedStackBefore);
    
    //stack.put(MemoryProfiling::getCurrentUsedStack());
    usedStackBefore = MemoryProfiling::getCurrentUsedStack();
    initNN();
    //l = stack.get();
    printf("--- DELTA initNN(): %i ---\n", usedStackEnd - usedStackBefore);

    unsigned int acquiredValues = 0;
    unsigned int valuesToAcquire = 1;//8*60*60/(((unsigned int)odr - 15)*32);
    //printf("values to acquire : %i \n", valuesToAcquire);
    while(true) 
    {
        // queue is the shared object between the producer (lps22hb) and the 
        // consumer (this class). The queue.get() method is a blocking method, 
        // if the queue is empty the thread keep waiting for a value to
        // to be inserted into the queue. As soon as the value is inserted, that 
        // value is returned by this method.
        //stack.put(MemoryProfiling::getCurrentUsedStack());
        usedStackBefore = MemoryProfiling::getCurrentUsedStack();
        float value = queue.get();
        //l = stack.get();
        printf("--- DELTA queue.get(): %i ---\n", usedStackEnd - usedStackBefore);
        // incremental mean formula: avg(n) = avg(n-1) + (newValue - avg(n-1))/n
        incrementalMean = incrementalMean + (value - incrementalMean)/(acquiredValues+1);
        acquiredValues++;
        
        // acquiredValues == 75 is used for testing purposes, this implementation 
        // will compute a prediction every 40 minutes. 
        // During delivery this condition should be acquiredValues == 900.
        // This is because every 32 seconds the FIFO in the sensor is full 
        // and so measurements must be collected. Since the predictions must be 
        // provided every 8h, it is necessary to average all the measurements 
        // acquired by the sensor (also for robustness w.r.t. noise). 
        // in 8h hours, there are 8x60x60=28800 seconds, 28800/32 = 900.
        // So, every time the FIFO has been emptied 900 times, 8 hours are passed.
        
        if (acquiredValues == valuesToAcquire) 
        {
            //8h has passed, time to predict
            enqueue(in_data, incrementalMean);

            //printf("Mean: %f\n", incrementalMean);
            //stack.put(MemoryProfiling::getCurrentUsedStack());
            usedStackBefore = MemoryProfiling::getCurrentUsedStack();    
            runNN(network, normalizeInput(in_data));
            //l = stack.get();
            printf("--- DELTA runNN(): %i ---\n", usedStackEnd - usedStackBefore);

            //printf("Prediction result: %f\n", denormalizeOutput(nn_outdata[0]));
            acquiredValues = 0;
            incrementalMean = 0;
        }
    }
}
#endif

int main()
{   
    initRCC();
#ifdef MAIN_PROFILING
    miosix::Thread::create(_main,2048);
    for(;;) Thread::sleep(1);
#else
    miosix::Thread::create(nn,2048);
    for(;;)
    {
        queue.put(1023.3f);
        Thread::sleep(10000);
    }
#endif
    
}