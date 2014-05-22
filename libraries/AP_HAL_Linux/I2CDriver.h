
#ifndef __AP_HAL_LINUX_I2CDRIVER_H__
#define __AP_HAL_LINUX_I2CDRIVER_H__

#include <AP_HAL_Linux.h>

class Linux::LinuxI2CDriver : public AP_HAL::I2CDriver {
public:
    LinuxI2CDriver();

    void begin();
    void end();
    void setTimeout(uint16_t ms);
    void setHighSpeed(bool active);

    /* write: for i2c devices which do not obey register conventions */
    uint8_t write(uint8_t addr, uint8_t len, uint8_t* data);
    /* writeRegister: write a single 8-bit value to a register */
    uint8_t writeRegister(uint8_t addr, uint8_t reg, uint8_t val);
    /* writeRegisters: write bytes to contigious registers */
    uint8_t writeRegisters(uint8_t addr, uint8_t reg,
                                   uint8_t len, uint8_t* data);

    /* read: for i2c devices which do not obey register conventions */
    uint8_t read(uint8_t addr, uint8_t len, uint8_t* data);
    /* readRegister: read from a device register - writes the register,
     * then reads back an 8-bit value. */
    uint8_t readRegister(uint8_t addr, uint8_t reg, uint8_t* data);

    /* readRegister: read contigious device registers - writes the first 
     * register, then reads back multiple bytes */
    uint8_t readRegisters(uint8_t addr, uint8_t reg,
                                  uint8_t len, uint8_t* data);

    uint8_t readRegistersMultiple(uint8_t addr, uint8_t reg,
                                  uint8_t len, uint8_t count, 
                                  uint8_t* data);

    uint8_t lockup_count();

    // substitute "take" in a multi-bus aware driver.
    // //      i2cdevice corresponds to the I2CDevice enum
    // bool lock_bus(uint32_t i2cdevice);
    // bool release_bus(uint32_t i2cdevice);
    bool lock_bus(uint8_t bus, uint32_t timeout_ms);
    bool lock_bus_nonblocking(uint8_t bus);
    bool release_bus(uint8_t bus);

    uint8_t num_buses();

    // THIS CALL SHOULDN'T BE USED WITH THE MULTI-BUS AWARE DRIVER
    AP_HAL::Semaphore* get_semaphore() { return NULL; }

private:
    bool set_address(uint8_t addr);
    int _fd;
    uint8_t _addr;

    typedef struct i2c_bus {
        char* device;
        int fd;
        LinuxSemaphore  semaphore;        
        // flags
    };
    
    uint8_t _num_i2c_buses = 3;
    // semaphores to init the different busses.
    LinuxSemaphore  i2cSemaphore0;
    LinuxSemaphore  i2cSemaphore1;
    LinuxSemaphore  i2cSemaphore2;

    i2c_bus _i2c_busses[] = {
        {
            "/dev/i2c-0",
            -1,
            i2cSemaphore0
        },
        {
            "/dev/i2c-1",
            -1,
            i2cSemaphore1
        },
        {
            "/dev/i2c-2",
            -1,
            i2cSemaphore2
        }
    };


    // // loop up for the I2CDevice
    // uint8_t lookup_device(uint32_t i2cdevice);
    
    // // probe for an I2C device in bus
    // uint8_t probe_device(uint8_t bus);

};

#endif // __AP_HAL_LINUX_I2CDRIVER_H__
