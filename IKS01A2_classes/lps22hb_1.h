/* 
 * File:   lps22hb.h
 * Author: nicolo
 *
 * Created on December 4, 2019, 6:48 PM
 */

#ifndef LPS22HB_H
#define	LPS22HB_H

#include "interfaces/gpio.h"
#include "arch/common/drivers/stm32f2_f4_i2c.h"
#include <kernel/scheduler/scheduler.h>
#include "miosix.h"

#define DEBUG

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

using namespace miosix;

static Thread *waiting;

void __attribute__((naked))EXTI15_10_IRQHandler()
{
    saveContext();
    asm volatile("bl _Z17EXTI10HandlerImplv");
    restoreContext();
}

void __attribute__((used))EXTI10HandlerImpl()
{
    EXTI->PR=EXTI_PR_PR10;
    if(waiting==nullptr) return;
    waiting->IRQwakeup();
    if(waiting->IRQgetPriority() > Thread::IRQgetCurrentThread()->IRQgetPriority())
    {
        Scheduler::IRQfindNextThread();
    }
    waiting=nullptr;
}


typedef enum {
  FIFO_BYPASS_MODE             	      = (unsigned char)0x00,	  /*!< The FIFO is disabled and empty. The pressure is read directly*/
  FIFO_MODE                           = (unsigned char)0x20,    /*!< Stops collecting data when full */
  STREAM_MODE                         = (unsigned char)0x40,    /*!< Keep the newest measurements in the FIFO*/
  FIFO_TRIGGER_STREAMTOFIFO_MODE      = (unsigned char)0x60,    /*!< STREAM MODE until trigger deasserted, then change to FIFO MODE*/
  FIFO_TRIGGER_BYPASSTOSTREAM_MODE    = (unsigned char)0x80,    /*!< BYPASS MODE until trigger deasserted, then STREAM MODE*/
  FIFO_TRIGGER_BYPASSTOFIFO_MODE      = (unsigned char)0xE0     /*!< BYPASS mode until trigger deasserted, then FIFO MODE*/
} FifoMode;

typedef enum {
  BDU_CONTINUOUS_UPDATE     =  (unsigned char)0x00,  /*!< Data updated continuously */
  BDU_NO_UPDATE             =  (unsigned char)0x02   /*!< Data updated after a read operation */
} BduMode;

typedef enum {
  ODR_ONE_SHOT   = (unsigned char)0x00,         /*!< Output Data Rate: one shot */
  ODR_1HZ        = (unsigned char)0x10,         /*!< Output Data Rate: 1Hz */
  ODR_10HZ       = (unsigned char)0x20,         /*!< Output Data Rate: 10Hz */
  ODR_25HZ       = (unsigned char)0x30,         /*!< Output Data Rate: 25Hz */
  ODR_50HZ       = (unsigned char)0x40,          /*!< Output Data Rate: 50Hz */
  ODR_75HZ       = (unsigned char)0x50          /*!< Output Data Rate: 75Hz */
} OdrMode;

typedef enum {
    CTRL_REG1      = (unsigned char) 0x10,
    CTRL_REG2      = (unsigned char) 0x11,
    CTRL_REG3      = (unsigned char) 0x12,
    CTRL_FIFO_REG  = (unsigned char) 0x14,
    ODR_MASK       = (unsigned char) 0x70,
    BDU_MASK       = (unsigned char) 0x02,
    FIFO_MODE_MASK = (unsigned char) 0xE0,
    FIFO_EN_MASK   = (unsigned char) 0x40,
    FIFO_EN_BIT    = (unsigned char) 6,
    FIFO_FULL_MASK = (unsigned char) 0x20,
    FIFO_FULL_BIT  = (unsigned char) 5,
    FIFO_OVR_MASK  = (unsigned char) 0x08,
    FIFO_OVR_BIT   = (unsigned char) 3
} UsefulAddresses;

template <typename SDA, typename SCL, unsigned stretchTimeout=50, bool fast=false>
class lps22hb_1 {
public:
    
    float readValues()
    {
        //i2c::sendStart();
        //i2c::send(LPS22HB_ADDR); 
        return 0.1;
    }
    
    void waitForFullFifo()
    {
        FastInterruptDisableLock dLock;
        waiting=Thread::IRQgetCurrentThread();
        while(waiting)
        {
            Thread::IRQwait();
            FastInterruptEnableLock eLock(dLock);
            Thread::yield();
        }
    }
    
    void init()
    {
        //set interrupt gpio pin to input mode
        int_fifo::mode(Mode::INPUT);
        
        EXTI->IMR |= EXTI_IMR_MR10;
        EXTI->FTSR |= EXTI_FTSR_TR10;
        
        //enable FIFO
        enableOrDisableFifo(1);
        //set FIFO mode
        //setFifoMode(FifoMode::STREAM_MODE);
        //set BDU to continuous mode
        //setBduMode(BduMode::BDU_CONTINUOUS_UPDATE); 
        //set ODR to 1Hz
        //setOdrFreq(OdrMode::ODR_75HZ);
        //enable interrupt and set priority
        //NVIC_EnableIRQ(IRQn_Type::EXTI15_10_IRQn);
        //NVIC_SetPriority(IRQn_Type::EXTI15_10_IRQn, 15);
        //NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
        //enable interrupt on overrun of the fifo
        //enableInterruptOnOvrFifo(1);
        //enable interrupt on full fifo
        //enableInterruptOnFullFifo(1);
    }
    
    lps22hb_1(){init();}
    
private:    
    I2C1Driver &i2c = I2C1Driver::instance();
    typedef Gpio<GPIOB_BASE,10> int_fifo;
        
    /*unsigned char static readByteOfReg(const unsigned char& reg)
    {
        i2c::sendStart();
        i2c::send(LPS22HB_ADDR_W);
        i2c::send(reg);
        i2c::sendRepeatedStart();
        i2c::send(LPS22HB_ADDR_R);
        unsigned char reg_val = i2c::recvWithNack();
        i2c::sendStop();
        return reg_val;
    }
    
    bool static writeByteToReg(const unsigned char& reg, const unsigned char& data)
    {
        i2c::sendStart();
        i2c::send(LPS22HB_ADDR_W);
        i2c::send(reg);
        bool lastAck = i2c::send(data);
        i2c::sendStop();
        return lastAck;
    }*/
    
    //mode = 1 -> enable, mode = 0 -> disable
    void enableOrDisableFifo(const unsigned char& mode)
    {
        unsigned char ctrl_reg2 = 0;
        i2c.recv(CTRL_REG2, (void *)&ctrl_reg2,1);
        if(DEBUG_TEST)  printf("FIFO enable before -> 0x%x \n", ctrl_reg2);
        ctrl_reg2 &= ~FIFO_EN_MASK;
        ctrl_reg2 |= mode << FIFO_EN_BIT;
        i2c.send(CTRL_REG2, (void *)&ctrl_reg2, 1, true);
        if(DEBUG_TEST){
            i2c.recv(CTRL_REG2, (void *)&ctrl_reg2,1);
            printf("FIFO enable after -> 0x%x \n", ctrl_reg2);
        }
        
        /*unsigned char ctrl_reg2 = readByteOfReg(CTRL_REG2);
        if(DEBUG_TEST)  printf("FIFO enable before -> 0x%x \n", ctrl_reg2);
        ctrl_reg2 &= ~FIFO_EN_MASK;
        ctrl_reg2 |= mode << FIFO_EN_BIT;
        writeByteToReg(CTRL_REG2, ctrl_reg2);
        if(DEBUG_TEST)  printf("FIFO enable after -> 0x%x \n", readByteOfReg(CTRL_REG2));*/
    }
    
    void setFifoMode(const unsigned char& mode)
    {
        unsigned char ctrl_fifo_reg = 0;
        i2c.recv(CTRL_REG2, &ctrl_fifo_reg,1);
        if(DEBUG_TEST)  printf("FIFO mode before -> 0x%x \n", ctrl_fifo_reg);
        ctrl_fifo_reg &= ~FIFO_MODE_MASK;
        ctrl_fifo_reg |= mode;
        i2c.send(CTRL_REG2, &ctrl_fifo_reg, 1, true);
        if(DEBUG_TEST){
            i2c.recv(CTRL_REG2, &ctrl_fifo_reg,1);
            printf("FIFO mode after -> 0x%x \n", ctrl_fifo_reg);
        }
        
        /*unsigned char ctrl_fifo_reg = readByteOfReg(CTRL_FIFO_REG);
        if(DEBUG_TEST)  printf("FIFO mode before -> 0x%x \n", ctrl_fifo_reg);
        ctrl_fifo_reg &= ~FIFO_MODE_MASK;
        ctrl_fifo_reg |= mode;
        writeByteToReg(CTRL_FIFO_REG, ctrl_fifo_reg);
        if(DEBUG_TEST) printf("FIFO mode after -> 0x%x \n", readByteOfReg(CTRL_FIFO_REG));*/
    }
    
    void setBduMode(const unsigned char& mode)
    {
        unsigned char ctrl_reg1 = 0;
        i2c.recv(CTRL_REG1, &ctrl_reg1,1);
        if(DEBUG_TEST)  printf("BDU value before -> 0x%x \n", ctrl_reg1);
        ctrl_reg1 &= ~BDU_MASK;
        ctrl_reg1 |= mode;
        i2c.send(CTRL_REG1, &ctrl_reg1, 1, true);
        if(DEBUG_TEST){
            i2c.recv(CTRL_REG1, &ctrl_reg1,1);
            printf("BDU value after -> 0x%x \n", ctrl_reg1);
        }
        
        /*unsigned char ctrl_reg1 = readByteOfReg(CTRL_REG1);
        if(DEBUG_TEST)  printf("BDU value before -> 0x%x \n", ctrl_reg1);
        ctrl_reg1 &= ~BDU_MASK;
        ctrl_reg1 |= mode;
        writeByteToReg(CTRL_REG1, ctrl_reg1);
        if(DEBUG_TEST)  printf("BDU value after -> 0x%x \n", readByteOfReg(CTRL_REG1));*/
    }
    
    void setOdrFreq(const unsigned char& mode)
    {
        unsigned char ctrl_reg1 = 0;
        i2c.recv(CTRL_REG1, &ctrl_reg1,1);
        if(DEBUG_TEST)  printf("ODR value before -> 0x%x \n", ctrl_reg1);
        ctrl_reg1 &= ~ODR_MASK;
        ctrl_reg1 |= mode;
        i2c.send(CTRL_REG1, &ctrl_reg1, 1, true);
        if(DEBUG_TEST){
            i2c.recv(CTRL_REG1, &ctrl_reg1,1);
            printf("ODR value after -> 0x%x \n", ctrl_reg1);
        }
        
        /*unsigned char ctrl_reg1 = readByteOfReg(CTRL_REG1);
        if(DEBUG_TEST)  printf("ODR value before -> 0x%x \n", ctrl_reg1);
        ctrl_reg1 &= ~ODR_MASK;
        ctrl_reg1 |= mode;
        writeByteToReg(CTRL_REG1, ctrl_reg1);
        if(DEBUG_TEST)  printf("ODR value after -> 0x%x \n", readByteOfReg(CTRL_REG1));*/
    }
    
    /*void enableInterruptOnOvrFifo(const unsigned char& mode)
    {
        unsigned char ctrl_reg3 = readByteOfReg(CTRL_REG3);
        if(DEBUG_TEST)  printf("FIFO ovr int before -> 0x%x \n", ctrl_reg3);
        ctrl_reg3 &= ~FIFO_OVR_MASK;
        ctrl_reg3 |= mode << FIFO_OVR_BIT;
        writeByteToReg(CTRL_REG3, ctrl_reg3);
        if(DEBUG_TEST)  printf("FIFO ovr int after -> 0x%x \n", readByteOfReg(CTRL_REG3));
    }*/
    
    void enableInterruptOnFullFifo(const unsigned char& mode)
    {
        unsigned char ctrl_reg3 = 0;
        i2c.recv(CTRL_REG3, &ctrl_reg3,1);
        if(DEBUG_TEST)  printf("FIFO full int before -> 0x%x \n", ctrl_reg3);
        ctrl_reg3 &= ~FIFO_FULL_MASK;
        ctrl_reg3 |= mode << FIFO_FULL_BIT;
        i2c.send(CTRL_REG3, &ctrl_reg3, 1, true);
        if(DEBUG_TEST){
            i2c.recv(CTRL_REG3, &ctrl_reg3,1);
            printf("FIFO full int after -> 0x%x \n", ctrl_reg3);
        }
        
        /*unsigned char ctrl_reg3 = readByteOfReg(CTRL_REG3);
        if(DEBUG_TEST)  printf("FIFO full int before -> 0x%x \n", ctrl_reg3);
        ctrl_reg3 &= ~FIFO_FULL_MASK;
        ctrl_reg3 |= mode << FIFO_FULL_BIT;
        writeByteToReg(CTRL_REG3, ctrl_reg3);
        if(DEBUG_TEST)  printf("FIFO full int after -> 0x%x \n", readByteOfReg(CTRL_REG3));*/
    }
};

#endif	/* LPS22HB_H */

