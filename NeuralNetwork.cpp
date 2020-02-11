    /* 
 * File:   NeuralNetwork_.cpp
 * Author: nicolo
 * 
 * Created on January 21, 2020, 7:14 PM
 */

#include "NeuralNetwork.h"
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <array>

NeuralNetwork::NeuralNetwork(SyncQueue<float> &queue): queue(queue)
{
    initNN();
};

NeuralNetwork::~NeuralNetwork() 
{
    if(quit.load()) return;
    ai_network_destroy(network);
    quit.store(true);
    t->join();
}

void NeuralNetwork::run() 
{
    while(!quit.load()) 
    {
        // queue is the shared object between the producer (lps22hb) and the 
        // consumer (this class). The queue.get() method is a blocking method, 
        // if the queue is empty the thread keep waiting for a value to
        // to be inserted into the queue. As soon as the value is inserted, that 
        // value is returned by this method.
        float value = queue.get();
        
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
        if (acquiredValues == 75) 
        {
            //8h has passed, time to predict
            enqueue(in_data, incrementalMean);

            printf("Mean: %f\n", incrementalMean);
            runNN(network, normalizeInput(in_data));
            printf("Prediction result: %f\n", denormalizeOutput(nn_outdata[0]));
            acquiredValues = 0;
            incrementalMean = 0;
        }
    }
}

void NeuralNetwork::initNN()
{
    ai_error err = ai_network_create(&network, (const ai_buffer*)AI_NETWORK_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE) 
    {
        printf("E: AI error in NN creation - type=%lu code=%lu\r\n", err.type, err.code);
        // TODO: error handling
    }
    else 
    {
        printf("NN successfully created.\n");
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
        printf("NN successfully initialized.\n");
    }
}

int  NeuralNetwork::runNN(ai_handle network, void* input)
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
    return 1;    
}

void NeuralNetwork::enqueue (ai_float* input, ai_float newValue)
{
    for (int i=0; i < AI_NETWORK_IN_1_SIZE-1; i++) 
    {
        input[i] = input[i+1];
    }
    input[AI_NETWORK_IN_1_SIZE - 1] = newValue; 
}

ai_float*  NeuralNetwork::normalizeInput(ai_float* input)
{
    for (int i=0; i < AI_NETWORK_IN_1_SIZE; i++) 
    {
        // if the value is between the pressure range of the sensor, it must be
        // normalized.
        if (input[i] > 260 && input[i] < 1260)
            input[i] = (input[i] - normMin) / (normMax - normMin);
        printf("element number %d of the queue before prediction: %f\n",i+1,input[i]);
    }
    return input;
}

float  NeuralNetwork::denormalizeOutput(float output)
{
    return output*(normMax - normMin) + normMin;
}

