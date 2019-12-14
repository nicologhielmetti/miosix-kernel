/* 
 * File:   lps22hb.h
 * Author: nicolo
 *
 * Created on December 4, 2019, 6:48 PM
 */

#ifndef LPS22HB_H
#define	LPS22HB_H

#include "interfaces/gpio.h"
#include "util/software_i2c.h"

#define DEBUG

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

using namespace miosix;

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

template <typename SDA, typename SCL, unsigned stretchTimeout=50, bool fast=false>
class lps22hb {
public:
    
    static float readValues()
    {
        i2c::sendStart();
        i2c::send(0xBA); //or 0xB8
        return 0.1;
    }
    
    static void init()
    {
        //init I2C
        i2c::init();
        
        //set FIFO mode
        setFifoMode(FifoMode::FIFO_MODE);
        //set BDU to continuous mode
        setBduMode(BduMode::BDU_CONTINUOUS_UPDATE); 
        //set ODR to 1Hz
        setOdrFreq(OdrMode::ODR_1HZ);
    }
    
private:    
    typedef SoftwareI2C<SDA, SCL, stretchTimeout, fast> i2c;
    
    static const unsigned char CTRL_REG1      = 0x10;
    static const unsigned char CTRL_REG2      = 0x11;
    static const unsigned char ODR_MASK       = 0x70;
    static const unsigned char BDU_MASK       = 0x02;
    static const unsigned char FIFO_MODE_MASK = 0xE0;
    static const unsigned char FIFO_EN_MASK   = 0x40;
    static const unsigned char FIFO_EN_BIT    = (unsigned char) 6;
    
    lps22hb();
    
    void static setFifoMode(unsigned char mode)
    {
        i2c::sendStart();
        if(DEBUG_TEST)
        {
            i2c::send(CTRL_REG2);
            unsigned char control = i2c::recvWithAck();
            printf("FIFO mode before -> 0x%x \n", control & FIFO_MODE_MASK);
        }
        i2c::send(CTRL_REG2);
        unsigned char ctrl_reg2 = i2c::recvWithAck();
        ctrl_reg2 &= ~FIFO_EN_MASK;
        ctrl_reg2 |= mode << FIFO_EN_BIT;
        i2c::send(ctrl_reg2);
        if(DEBUG_TEST)
        {
            i2c::send(CTRL_REG2);
            unsigned char control = i2c::recvWithAck();
            printf("FIFO mode after -> 0x%x \n", control & FIFO_MODE_MASK);
        }
        
    }
    
    void static setBduMode(unsigned char mode)
    {
        i2c::sendStart();
        if(DEBUG_TEST)
        {
            i2c::send(CTRL_REG1);
            unsigned char control = i2c::recvWithAck();
            printf("BDU mode before -> 0x%x \n", control & BDU_MASK);
        }
        i2c::send(CTRL_REG1);
        unsigned char ctrl_reg1 = i2c::recvWithAck();
        ctrl_reg1 &= ~BDU_MASK;
        ctrl_reg1 |= mode;
        i2c::send(ctrl_reg1);
        if(DEBUG_TEST)
        {
            i2c::send(CTRL_REG1);
            unsigned char control = i2c::recvWithAck();
            printf("BDU mode after -> 0x%x \n", control & BDU_MASK);
        }
    }
    
    void static setOdrFreq(unsigned char mode)
    {
        i2c::sendStart();
        if(DEBUG_TEST)
        {
            i2c::send(CTRL_REG1);
            unsigned char control = i2c::recvWithAck();
            printf("ODR mode before -> 0x%x \n", control & ODR_MASK);
        }
        i2c::send(CTRL_REG1);
        unsigned char ctrl_reg1 = i2c::recvWithAck();
        ctrl_reg1 &= ~ODR_MASK;
        ctrl_reg1 |= mode;
        i2c::send(ctrl_reg1);
        if(DEBUG_TEST)
        {
            i2c::send(CTRL_REG1);
            unsigned char control = i2c::recvWithAck();
            printf("ODR mode after -> 0x%x \n", control & ODR_MASK);
        }
    }
};

#endif	/* LPS22HB_H */

