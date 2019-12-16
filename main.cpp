//default baudrate 19200


#include <cstdio>
#include <iostream>
#include "miosix.h"
#include "lstm.h"
#include "lstm_data.h"
#include "util/software_i2c.h"
#include "IKS01A2_classes/lps22hb.h"



using namespace std;
using namespace miosix;

int runLSTM(ai_handle network, void* input, void* output);

void initRCC()
{
    //enable RCC
    FastInterruptDisableLock a;
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
    CRC->CR = CRC_CR_RESET;
}


static ai_handle lstm = AI_HANDLE_NULL;
static ai_u8 activations[AI_LSTM_DATA_ACTIVATIONS_SIZE];
static ai_buffer nn_input[AI_LSTM_IN_NUM] =  AI_LSTM_IN;
static ai_buffer nn_output[AI_LSTM_OUT_NUM] =  AI_LSTM_OUT;
static ai_float nn_outdata[AI_LSTM_OUT_1_SIZE];


int main()
{

    initRCC();


    typedef Gpio<GPIOB_BASE,8>  scl;
    typedef Gpio<GPIOB_BASE,9>  sda;
    typedef Gpio<GPIOA_BASE,5> led;
    typedef lps22hb<sda,scl> pressure_sensor;

    //pressure_sensor::init();

    ai_error err = ai_lstm_create(&lstm, (ai_buffer*)AI_LSTM_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE) {
        printf("E: AI error - type=%lu code=%lu\r\n", err.type, err.code);
    }

    //ai_u8 activations[AI_LSTM_DATA_ACTIVATIONS_SIZE];

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
    printf("%f\n", output[0]);
    runLSTM(lstm, input, output);
    printf("%f\n", output[0]);

    ai_lstm_destroy(lstm);
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
