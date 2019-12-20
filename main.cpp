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
#include "miosix.h"
#include "network.h"
#include "network_data.h"
#include "util/software_i2c.h"
#include "IKS01A2_classes/lps22hb.h"



using namespace std;
using namespace miosix;

// AI-related functions
int runNN(ai_handle network, void* input);
ai_float* normalizeInput(ai_float* input);
float denormalizeOutput(float output);

// AI-related variables
ai_handle network = AI_HANDLE_NULL;
static ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
static ai_buffer nn_input[AI_NETWORK_IN_NUM] =  AI_NETWORK_IN;
static ai_buffer nn_output[AI_NETWORK_OUT_NUM] =  AI_NETWORK_OUT;
static ai_float nn_outdata[AI_NETWORK_OUT_1_SIZE];

// hard-coded normalization parameters obtained at training time
struct NormPar {
    float min;
    float max;
} nnNormPar = {978.52708333, 1040.8893617};

void initRCC();

int main()
{
    initRCC();
    
    typedef Gpio<GPIOB_BASE,8> scl;
    typedef Gpio<GPIOB_BASE,9> sda;
    typedef Gpio<GPIOA_BASE,5> led;
    typedef lps22hb<sda,scl> pressure_sensor;

    pressure_sensor::init();

    // network creation
    ai_error err = ai_network_create(&network, (const ai_buffer*)AI_NETWORK_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE) {
        printf("E: AI error in NN creation - type=%lu code=%lu\r\n", err.type, err.code);
        // TODO: error handling
    } else {
        printf("NN successfully created.\n");
    }
    
    // network initialization
    const ai_network_params params = {
       AI_NETWORK_DATA_WEIGHTS(ai_network_data_weights_get()),
       AI_NETWORK_DATA_ACTIVATIONS(activations) };

    if (!ai_network_init(network, &params)) {
        err = ai_network_get_error(network);
        printf("E: AI error in NN initialization - type=%lu code=%lu\r\n", err.type, err.code);
        // TODO: error handling
    } else {
       printf("NN successfully initialized.\n");
    }

    // input examples
    static ai_float in_data[AI_NETWORK_IN_1_SIZE] = {1005.5957446808509, 1006.9659574468085, 1009.7425531914895};
    
    // network prediction
    runNN(network, normalizeInput(in_data));
    printf("Prediction result: %f\n", denormalizeOutput(nn_outdata[0]));
    
    // network deallocation
    ai_network_destroy(network);
 }


int runNN(ai_handle network, void* input) {
    ai_i32 nbatch;
    ai_error err;

    //ai_buffer ai_input[AI_NETWORK_IN_NUM] = AI_NETWORK_IN ;
    //ai_buffer ai_output[AI_NETWORK_OUT_NUM] = AI_NETWORK_OUT ;

    nn_input[0].n_batches = 1;
    nn_input[0].data = AI_HANDLE_PTR(input);
    nn_output[0].n_batches = 1;
    nn_output[0].data = AI_HANDLE_PTR(nn_outdata);

    nbatch = ai_network_run(network, &nn_input[0], &nn_output[0]);
    if (nbatch != 1) {
        err = ai_network_get_error(network);
        //TODO: handle error
    }

    return 1;
}

ai_float* normalizeInput(ai_float* input) {
    for (int i=0; i < AI_NETWORK_IN_1_SIZE; i++) {
        input[i] = (input[i] - nnNormPar.min) / (nnNormPar.max - nnNormPar.min);
    }
    return input;
}

float denormalizeOutput(float output) {
    return output*(nnNormPar.max - nnNormPar.min) + nnNormPar.min;
}


/*
    typedef Gpio<GPIOA_BASE,5> led;

    for(;;)
    {
        led::high();
        Thread::sleep(1000);
        led::low();
        Thread::sleep(1000);
    }
 */

void initRCC(){
    //enable CRC pheripherals
    FastInterruptDisableLock a;
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
    CRC->CR = CRC_CR_RESET;
}