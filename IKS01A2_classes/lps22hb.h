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
#include <kernel/scheduler/scheduler.h>
#include "miosix.h"


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

typedef enum {
    CTRL_REG1        = (unsigned char)0x10,
    CTRL_REG2        = (unsigned char)0x11,
    CTRL_REG3        = (unsigned char)0x12,
    CTRL_FIFO_REG    = (unsigned char)0x14,
    ODR_MASK         = (unsigned char)0x70,
    BDU_MASK         = (unsigned char)0x02,
    FIFO_MODE_MASK   = (unsigned char)0xE0,
    FIFO_EN_MASK     = (unsigned char)0x40,
    FIFO_EN_BIT      = (unsigned char)6,
    FIFO_FULL_MASK   = (unsigned char)0x20,
    FIFO_FULL_BIT    = (unsigned char)5,
    FIFO_OVR_MASK    = (unsigned char)0x08,
    FIFO_OVR_BIT     = (unsigned char)3,
    ADD_INC_MASK     = (unsigned char)0x10,
    ADD_INC_BIT      = (unsigned char)4,
    PRESS_OUT_XL_REG = (unsigned char)0x28,
    BOOT_BIT         = (unsigned char)7
    //ADDR_W           = (unsigned char)0xBA,
    //ADDR_R           = (unsigned char)0xBB
} UsefulAddresses;


template <typename SDA, typename SCL, unsigned stretchTimeout=50, bool fast=false>
class lps22hb {
public:
    int hasDataToRead();
    
    float getLast32AvgPressure();
    
    void waitForFullFifo();
    
    void init();
    
    lps22hb(const unsigned char &addr);
    
private:    
    typedef SoftwareI2C<SDA, SCL, stretchTimeout, fast> i2c;
    typedef Gpio<GPIOB_BASE,10> int_fifo;
    typedef Gpio<GPIOA_BASE,5>  led;
    unsigned char addr;
    
    unsigned char readByteOfReg(const unsigned char& reg);
    
    bool writeByteToReg(const unsigned char& reg, const unsigned char& data);
    
    void rebootSensors();
    
    void setAutoIncRegAddr(const unsigned char& mode);
    
    //mode = 1 -> enable, mode = 0 -> disable
    void enableFifo(const unsigned char& mode);
    
    void setFifoMode(const unsigned char& mode);
    
    void setBduMode(const unsigned char& mode);
    
    void setOdrFreq(const unsigned char& mode);
    
    void enableInterruptOnOvrFifo(const unsigned char& mode);
    
    void enableInterruptOnFullFifo(const unsigned char& mode);
};

#endif	/* LPS22HB_H */

