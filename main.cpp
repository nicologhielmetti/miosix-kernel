
#include <cstdio>
#include "miosix.h"
#include "lstm.h"
#include "util/software_i2c.h"


using namespace std;
using namespace miosix;

int main()
{
    {
        FastInterruptDisableLock a;
        RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
        CRC->CR = CRC_CR_RESET;
    }
    
    ai_handle* network;
    ai_buffer* config;
    ai_buffer* input;
    ai_buffer* output;
    ai_network_params* params;
    
    ai_error err = ai_lstm_create(network, config);
    ai_bool bl = ai_lstm_init(network, params);
    ai_i32 i32 = ai_lstm_run(network, input, output);
    ai_lstm_destroy(network);
    
 }
