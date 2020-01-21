/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NeuralNetwork.h
 * Author: Francesco Alongi
 *
 * Created on 21 gennaio 2020, 10.38
 */

#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "../SyncQueue/SyncQueue.h"
#include "../ActiveObject/ActiveObject.h"

class NeuralNetwork : public ActiveObject {    
public:
    NeuralNetwork(const SyncQueue<float> &queue, float &normMin, float &normMax) 
                : queue(queue), normMin(normMin), normMax(normMax)  {
        initNN();
    };
    virtual ~NeuralNetwork() {
        ai_network_destroy(network);
    };
    void run() {
        while(quit.load()) {
            float value = queue.get();
            if (acquiredValues == 0) {
                incrementalMean = value;
                acquiredValues++;
            } else {
                // incremental mean formula: avg(n) = avg(n-1) + (newValue - avg(n-1))/n
                acquiredValues++;
                incrementalMean = incrementalMean + (value - incrementalMean)/acquiredValues;
            }
            if (acquiredValues == 900) {
                //8h has passed, time to predict
                enqueue(in_data, incrementalMean);
                runNN(network, normalizeInput(in_data));
                printf("Prediction result: %f\n", denormalizeOutput(nn_outdata[0]));
                acquiredValues = 0;
            }
            
        }
    }
private:
    const SyncQueue<float> queue;
    int acquiredValues = 0;
    ai_float incrementalMean;
    const float normMin;
    const float normMax;
    ai_float in_data[AI_NETWORK_IN_1_SIZE] = {0, 0, 0};
    ai_handle network = AI_HANDLE_NULL;
    ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
    ai_buffer nn_input[AI_NETWORK_IN_NUM] =  AI_NETWORK_IN;
    ai_buffer nn_output[AI_NETWORK_OUT_NUM] =  AI_NETWORK_OUT;
    ai_float nn_outdata[AI_NETWORK_OUT_1_SIZE];

    void enqueue (ai_float* input, ai_float newValue) {
        for (int i=1; i < AI_NETWORK_IN_1_SIZE; i++) {
            input[i-1] = input[i];
        }
        input[AI_NETWORK_IN_1_SIZE - 1] = newValue;
    }
    ai_float* normalizeInput(ai_float* input) {
        for (int i=0; i < AI_NETWORK_IN_1_SIZE; i++) {
            input[i] = (input[i] - normMin) / (normMax - normMin);
        }
        return input;
    }
    float denormalizeOutput(float output) {
        return output*(normMax - normMin) + normMin;
    }
    void initNN() {
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
    }
    int runNN(ai_handle network, void* input) {
        ai_i32 nbatch;
        ai_error err;

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
    NeuralNetwork(const NeuralNetwork& orig) = delete;
    NeuralNetwork& operator=(const NeuralNetwork& orig) = delete;
};

#endif /* NEURALNETWORK_H */

