//default baudrate 19200


#include <cstdio>
#include <iostream>
#include "miosix.h"
#include "lstm.h"
#include "IKS01A2_classes/lps22hb.h"

using namespace std;
using namespace miosix;

void initRCC()
{
    //enable RCC
    FastInterruptDisableLock a;
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
    CRC->CR = CRC_CR_RESET;
}


int main()
{
    
    initRCC();
    //NN variables declaration and initialization
    /*ai_handle* network = AI_HANDLE_NULL;
    ai_buffer* config;
    ai_buffer* input;
    ai_buffer* output;
    ai_network_params* params;*/
    
    //GPIO for I2C1 declaration
    typedef Gpio<GPIOB_BASE,8>  scl;
    typedef Gpio<GPIOB_BASE,9>  sda;
    typedef Gpio<GPIOA_BASE,5> led;
    typedef lps22hb<sda,scl> pressure_sensor;
    
    
    //NN variables initialization
    /*ai_error err = ai_lstm_create(network, config);
    ai_bool bl = ai_lstm_init(network, params);
    ai_i32 i32 = ai_lstm_run(network, input, output);
    ai_lstm_destroy(network);*/
    
    pressure_sensor::init();
    
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