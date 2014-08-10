

#ifndef __AP_HAL_EMPTY_EEPROM_H__
#define __AP_HAL_EMPTY_EEPROM_H__

#include <AP_HAL.h>
#include "AP_HAL_Empty_Namespace.h"

class Empty::EmptyEEPROM : public AP_HAL::EEPROM
{
public:
    int8_t init(void* machtnichts);
    int8_t _register_write( uint32_t* src, uint16_t addr, uint16_t len );
    int8_t _register_write( uint16_t* src, uint16_t addr, uint16_t len );
    int8_t _register_write( uint8_t* src, uint16_t addr, uint16_t len );
    int8_t _register_read( uint16_t addr, uint8_t opcode );
};

#endif // __AP_HAL_LINUX_STORAGE_H__
