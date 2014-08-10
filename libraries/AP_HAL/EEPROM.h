
#ifndef __AP_HAL_EEPROM_H__
#define __AP_HAL_EEPROM_H__

#include "AP_HAL_Namespace.h"

class AP_HAL::EEPROM
{
public:
    virtual int8_t init(void* machtnichts) = 0;
    virtual int8_t _register_write( uint32_t* src, uint16_t addr, uint16_t len ) = 0;
    virtual int8_t _register_write( uint16_t* src, uint16_t addr, uint16_t len ) = 0;
    virtual int8_t _register_write( uint8_t* src, uint16_t addr, uint16_t len ) = 0;
    virtual int8_t _register_read( uint16_t addr, uint8_t opcode ) = 0;
  
};

#endif