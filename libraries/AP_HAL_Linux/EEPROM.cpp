#include <AP_HAL.h>
#if CONFIG_HAL_BOARD == HAL_BOARD_LINUX

#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include "EEPROM.h"
using namespace Linux;

extern const AP_HAL::HAL& hal;

LinuxEEPROM::LinuxEEPROM() :
_spi(NULL),
_spi_sem(NULL),
_initialised(false)
	{};
int8_t LinuxEEPROM::init(void* machtnichts)
{
	if(_initialised){
		return 0;
	}
	uint8_t manufacturerID;
    _spi = hal.spi->device(AP_HAL::SPIDevice_Dataflash);
	uint8_t signature[4] = {0x00,0xaf,0xf0,0x0f};
	uint8_t i = 0;
	for(int i=0;true;i++){
		manufacturerID = _register_read(0,OPCODE_RDID);

		if(manufacturerID == 0x7F){
			break;
		}
		else{
			hal.scheduler->delay(1000);
		}
		if(i==4){
			hal.scheduler->panic(PSTR("FRAM: Failed to receive Manufacturer ID 5 times"));
		}
	}
	while(i<4){
		if(_register_read(i+4100,OPCODE_READ) == -1){
			continue;
		}
		else if((uint8_t)_register_read(i+4100,OPCODE_READ) != signature[i]){
			while(_register_write(signature,4100,4) == -1);
			return -1;
		}
		else{
			i++;
		}
	}
	_initialised = true;
	hal.console->println("FRAM: Online");
	return 0;
}
//FRAM I/O functions

int8_t LinuxEEPROM::_register_write( uint32_t* src, uint16_t addr, uint16_t len )
{

    uint8_t *tx;
    uint8_t *rx;
    uint16_t i;
	tx = new uint8_t[len+3];
	rx = new uint8_t[len+3];
	if(_write_enable(true) == -1){
		return -1;
	}
	tx[0] = OPCODE_WRITE;
    tx[1] = addr>>8;
    tx[2] = addr;

	for(i=0;i<len;i+=4){
		tx[i+3]=src[i]>>24;
		tx[i+4]=src[i]>>16;
		tx[i+5]=src[i]>>8;
		tx[i+6]=src[i];
	}
 
	if(transaction(tx, rx, len+3) == -1){
		return -1;
	}
	if(_write_enable(false) == -1){
		return -1;
	}
	return len;
}
int8_t LinuxEEPROM::_register_write( uint16_t* src, uint16_t addr, uint16_t len ){

    uint8_t *tx;
    uint8_t *rx;
    uint16_t i;
	tx = new uint8_t[len+3];
	rx = new uint8_t[len+3];
	if(_write_enable(true) == -1){
		return -1;
	}
	tx[0] = OPCODE_WRITE;
    tx[1] = addr>>8;
    tx[2] = addr;

	for(i=0;i<len;i+=2){
		tx[i+3]=src[i]>>8;
		tx[i+4]=src[i];
	}
	if(transaction(tx, rx, len+3) == -1){
		return -1;
	}
	if(_write_enable(false) == -1){
		return -1;
	}
	return len;
}
int8_t LinuxEEPROM::_register_write( uint8_t* src, uint16_t addr, uint16_t len ){

    uint8_t *tx;
    uint8_t *rx;
    uint16_t i;
	tx = new uint8_t[len+3];
	rx = new uint8_t[len+3];
	_write_enable(true);
	tx[0] = OPCODE_WRITE;
    tx[1] = addr>>8;
    tx[2] = addr;
	
    for(i=0;i<len;i++){
		tx[i+3] = src[i];
	}
	if(transaction(tx, rx, len+3) == -1){
		return -1;
	}
	if(_write_enable(false) == -1){
		return -1;
	}
	return len;
}
int8_t LinuxEEPROM::_write_enable(bool enable)
{
    uint8_t tx[2];
    uint8_t rx[2];
	if(enable){
	    tx[0] = OPCODE_WREN;
	    tx[1] = 0;
	    return transaction(tx, rx, 2);
	}
	else{
	    tx[0] = OPCODE_WRDI;
	    tx[1] = 0;
	    return transaction(tx, rx, 2);
	}
	
}

int8_t LinuxEEPROM::_register_read( uint16_t addr, uint8_t opcode )
{

    uint8_t tx[4];
    uint8_t rx[4];
 
    tx[0] = opcode;
    tx[1] = addr>>8;
    tx[2] = addr;
	tx[3] = 0;
    if(transaction(tx, rx, 4) == -1){
    	return -1;
    }
    return rx[3];
}

int8_t LinuxEEPROM::transaction(uint8_t* tx, uint8_t* rx, uint16_t len){
	_spi_sem = _spi->get_semaphore();
    if (!_spi_sem->take(100)) {
       // hal.console->printf_P(PSTR("FRAM: Unable to get semaphore"));
		return -1;
    }
    _spi->transaction(tx, rx, len);
	_spi_sem->give();
	return 0;
}

#endif // CONFIG_HAL_BOARD
