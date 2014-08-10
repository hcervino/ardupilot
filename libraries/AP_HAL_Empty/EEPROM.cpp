#include <AP_HAL.h>
#include "EEPROM.h"
using namespace Empty;

extern const AP_HAL::HAL& hal;

int8_t EmptyEEPROM::init(void* machtnichts)
{
	return 0;
}

int8_t EmptyEEPROM::_register_write( uint32_t* src, uint16_t addr, uint16_t len )
{
	return len;
}
int8_t EmptyEEPROM::_register_write( uint16_t* src, uint16_t addr, uint16_t len ){
	return len;
}
int8_t EmptyEEPROM::_register_write( uint8_t* src, uint16_t addr, uint16_t len ){
	return len;
}

int8_t EmptyEEPROM::_register_read( uint16_t addr, uint8_t opcode )
{
    return 0;
}

