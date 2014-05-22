
#include <AP_HAL.h>

#if CONFIG_HAL_BOARD == HAL_BOARD_LINUX || CONFIG_HAL_BOARD == HAL_BOARD_ERLE
#include "I2CDriver.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#ifndef I2C_SMBUS_BLOCK_MAX
#include <linux/i2c.h>
#endif

using namespace Linux;

/*
  constructor
 */
LinuxI2CDriver::LinuxI2CDriver(AP_HAL::Semaphore* semaphore) : 
    _semaphore(semaphore),
    _fd(-1)
{
}

/*
  called from HAL class init()
 */
void LinuxI2CDriver::begin() 
{
    for (uint8_t i=0;i<_num_i2c_buses;i++){
        if (_i2c_busses[i].fd != -1) {
            close(_i2c_busses[i].fd);
        }
        _fd = open(_i2c_busses[i].device, O_RDWR);        
    }
}

void LinuxI2CDriver::end() 
{
    for (uint8_t i=0;i<_num_i2c_buses;i++){
        if (_i2c_busses[i].fd != -1) {
            ::close(_i2c_busses[i].fd);
            _i2c_busses[i].fd = -1;
        }
    }
}

/*
  tell the I2C library what device we want to talk to
 */
bool LinuxI2CDriver::set_address(uint8_t addr)
{
    if (_fd == -1) {
        return false;
    }
    if (_addr != addr) {
        ioctl(_fd, I2C_SLAVE, addr);
        _addr = addr;
    }
    return true;
}

void LinuxI2CDriver::setTimeout(uint16_t ms) 
{
    // unimplemented
}

void LinuxI2CDriver::setHighSpeed(bool active) 
{
    // unimplemented    
}

uint8_t LinuxI2CDriver::write(uint8_t addr, uint8_t len, uint8_t* data)
{
    if (!set_address(addr)) {
        return 1;
    }
    if (::write(_fd, data, len) != len) {
        return 1;
    }
    return 0; // success
}


uint8_t LinuxI2CDriver::writeRegisters(uint8_t addr, uint8_t reg,
                                       uint8_t len, uint8_t* data)
{
    uint8_t buf[len+1];
    buf[0] = reg;
    if (len != 0) {
        memcpy(&buf[1], data, len);
    }
    return write(addr, len+1, buf);
}

/*
  this is a copy of i2c_smbus_access() from i2c-dev.h. We need it for
  platforms with older headers
 */
static inline __s32 _i2c_smbus_access(int file, char read_write, __u8 command, 
                                      int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;
	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;
	return ioctl(file,I2C_SMBUS,&args);
}

uint8_t LinuxI2CDriver::writeRegister(uint8_t addr, uint8_t reg, uint8_t val)
{
    if (!set_address(addr)) {
        return 1;
    }
    union i2c_smbus_data data;
    data.byte = val;
    if (_i2c_smbus_access(_fd,I2C_SMBUS_WRITE, reg,
                         I2C_SMBUS_BYTE_DATA, &data) == -1) {
        return 1;
    }
    return 0;
}

uint8_t LinuxI2CDriver::read(uint8_t addr, uint8_t len, uint8_t* data)
{
    if (!set_address(addr)) {
        return 1;
    }
    if (::read(_fd, data, len) != len) {
        return 1;
    }
    return 0;
}

uint8_t LinuxI2CDriver::readRegisters(uint8_t addr, uint8_t reg,
                                      uint8_t len, uint8_t* data)
{
    struct i2c_msg msgs[] = {
        {
        addr  : addr,
        flags : 0,
        len   : 1,
        buf   : &reg
        },
        {
        addr  : addr,
        flags : I2C_M_RD,
        len   : len,
        buf   : data,
        }
    };
    struct i2c_rdwr_ioctl_data i2c_data = {
    msgs : msgs,
    nmsgs : 2
    };

    if (ioctl(_fd, I2C_RDWR, &i2c_data) == -1) {
        return 1;
    }

    return 0;
}


uint8_t LinuxI2CDriver::readRegistersMultiple(uint8_t addr, uint8_t reg,
                                              uint8_t len, 
                                              uint8_t count, uint8_t* data)
{
    while (count > 0) {
        uint8_t n = count>8?8:count;
        struct i2c_msg msgs[2*n];
        struct i2c_rdwr_ioctl_data i2c_data = {
        msgs : msgs,
        nmsgs : 2*n
        };
        for (uint8_t i=0; i<n; i++) {
            msgs[i*2].addr = addr;
            msgs[i*2].flags = 0;
            msgs[i*2].len = 1;
            msgs[i*2].buf = &reg;
            msgs[i*2+1].addr = addr;
            msgs[i*2+1].flags = I2C_M_RD;
            msgs[i*2+1].len = len;
            msgs[i*2+1].buf = data;
            data += len;
        };
        if (ioctl(_fd, I2C_RDWR, &i2c_data) == -1) {
            return 1;
        }
        count -= n;
    }
    return 0;
}


uint8_t LinuxI2CDriver::readRegister(uint8_t addr, uint8_t reg, uint8_t* data)
{
    if (!set_address(addr)) {
        return 1;
    }
    union i2c_smbus_data v;
    if (_i2c_smbus_access(_fd,I2C_SMBUS_READ, reg,
                          I2C_SMBUS_BYTE_DATA, &v)) {
        return 1;
    }
    *data = v.byte;
    return 0;
}

uint8_t LinuxI2CDriver::lockup_count() 
{
    return 0;
}

uint8_t LinuxI2CDriver::lookup_device(uint32_t i2cdevice){
    return 0;
}

// bool LinuxI2CDriver::lock_bus(uint32_t i2cdevice, uint32_t timeout_ms){
bool LinuxI2CDriver::lock_bus(uint8_t bus, uint32_t timeout_ms){
    // TODO Figure out which is the device and select the appropiate instance in _i2c_busses
    AP_HAL::Semaphore _semaphore = _i2c_busses[bus].semaphore;
    if (!_semaphore.take(timeout_ms))
        return false;
    return true;
}

bool LinuxI2CDriver::lock_bus_nonblocking(uint8_t bus){
    // TODO Figure out which is the device and select the appropiate instance in _i2c_busses
    AP_HAL::Semaphore _semaphore = _i2c_busses[bus].semaphore;
    if (!_semaphore.take_nonblocking())
        return false;
    return true;
}

// bool LinuxI2CDriver::release_bus(uint32_t i2cdevice){    
bool release_bus(uint8_t bus){
    // TODO Figure out which is the device and select the appropiate instance in _i2c_busses    
    AP_HAL::Semaphore _semaphore = _i2c_busses[bus].semaphore;
    _semaphore.give();
    return true;
}

uint8_t LinuxI2CDriver::num_buses(){
    return _num_i2c_buses;
}

// uint8_t lookup_device(uint32_t i2cdevice){
//     return 0;
// }


// uint8_t probe_device(uint8_t bus){
//     return 0;
// }


#endif // CONFIG_HAL_BOARD
