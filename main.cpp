
#include <cstdio>
#include "miosix.h"
#include "lstm.h"
#include "lstm_data.h"
#include "util/software_i2c.h"


using namespace std;
using namespace miosix;

int runLSTM(ai_handle network, void* input, void* output);


int main()
{  

    {
        FastInterruptDisableLock a;
        RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
        CRC->CR = CRC_CR_RESET;
    }
    
    ai_handle lstm = AI_HANDLE_NULL;
    ai_error err = ai_lstm_create(&lstm, (ai_buffer*)AI_LSTM_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE) {
        printf("E: AI error - type=%lu code=%lu\r\n", err.type, err.code);
    }
    
    ai_u8 activations[AI_LSTM_DATA_ACTIVATIONS_SIZE];

    // network initialization
    const ai_network_params params = {
       AI_LSTM_DATA_WEIGHTS(ai_lstm_data_weights_get()),
       AI_LSTM_DATA_ACTIVATIONS(activations) };
    
    if (!ai_lstm_init(lstm, &params)) {
        err = ai_lstm_get_error(lstm);
        // TODO: error handling
    }
    
    float input[3] = {3,4,5};
    float output[1] = {0};
    runLSTM(lstm, input, output);
    ai_lstm_destroy(lstm);
    
    iprintf("%f\n", output[0]);
 }


int runLSTM(ai_handle network, void* input, void* output) {
    ai_i32 nbatch;
    ai_error err;
    
    ai_buffer ai_input[AI_LSTM_IN_NUM] = AI_LSTM_IN ;
    ai_buffer ai_output[AI_LSTM_OUT_NUM] = AI_LSTM_OUT ;
    
    ai_input[0].n_batches = 1;
    ai_input[0].data = AI_HANDLE_PTR(input);
    ai_output[0].n_batches = 1;
    ai_output[0].data = AI_HANDLE_PTR(output);
    
    nbatch = ai_lstm_run(network, &ai_input[0], &ai_output[0]);
    if (nbatch != 1) {
        err = ai_lstm_get_error(network);
        return err.code;
    }
    
    return 0;
}