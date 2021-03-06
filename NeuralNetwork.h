/* 
 * File:   NeuralNetwork_.h
 * Author: nicolo
 *
 * Created on January 21, 2020, 7:14 PM
 */

#ifndef NEURALNETWORK_H
#define	NEURALNETWORK_H

#include "SyncQueue.h"
#include "ActiveObject.h"
#include "NN/Inc/network.h"
#include "NN/Inc/network_data.h"
#include "UsefulTypedefs.h"

class NeuralNetwork : public ActiveObject {
public:
    NeuralNetwork(SyncQueue<float> &queue, const OdrMode& odr);
    ~NeuralNetwork();
    
private:
    SyncQueue<float>& queue;
    unsigned int acquiredValues = 0;
    ai_float incrementalMean = 0;
    OdrMode odr;
    ai_float in_data[AI_NETWORK_IN_1_SIZE] = {0, 0, 0};
    ai_handle network = AI_HANDLE_NULL;
    ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
    ai_buffer nn_input[AI_NETWORK_IN_NUM] =  AI_NETWORK_IN;
    ai_buffer nn_output[AI_NETWORK_OUT_NUM] =  AI_NETWORK_OUT;
    ai_float nn_outdata[AI_NETWORK_OUT_1_SIZE];
    
    // hard-coded normalization parameters obtained at training time
    const float normMin = 978.52708333;
    const float normMax = 1040.8893617;
    
    void initNN();
    void run();
    int runNN(ai_handle network, void* input);
    void enqueue (ai_float* input, ai_float newValue);
    ai_float* normalizeInput(ai_float* input);
    float denormalizeOutput(float output);
    NeuralNetwork(const NeuralNetwork& orig) = delete;
    NeuralNetwork& operator=(const NeuralNetwork& orig) = delete;
    

};

#endif	/* NEURALNETWORK__H */

