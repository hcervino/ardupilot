#include <AP_HAL.h>

#if CONFIG_HAL_BOARD == HAL_BOARD_LINUX

#include "RCInput.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <signal.h>
using namespace Linux;

static const AP_HAL::HAL& hal = AP_HAL_BOARD_DRIVER;

LinuxRCInput::LinuxRCInput() :
new_rc_input(false)
{}

static void catch_sigbus(int sig) {
  hal.scheduler->panic("RCInput.cpp:SIGBUS error generated\n");
}

void LinuxRCInput::init(void* machtnichts){
  uint32_t mem_fd;
  signal(SIGBUS,catch_sigbus);
  mem_fd = open("/dev/mem", O_RDWR|O_SYNC);
  rcin = (struct rcinput *) mmap(0, 0x1000, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd,PRU0_SHARED_BASE );
  close(mem_fd);
}

bool LinuxRCInput::new_input() 
{
    return new_rc_input;
}

uint8_t LinuxRCInput::num_channels() 
{
    return 8;
}

uint16_t LinuxRCInput::read(uint8_t ch) 
{
    new_rc_input = false;
    if (_override[ch]) {
        return _override[ch];
    }
    if (ch == 2) {
        // force low throttle for now
        return 900;
    }
    return 1500;
}

uint8_t LinuxRCInput::read(uint16_t* periods, uint8_t len) 
{
    for (uint8_t i=0; i<len; i++) {
	periods[i] = read(i);
    }
    return 8;
}

bool LinuxRCInput::set_overrides(int16_t *overrides, uint8_t len) 
{
    bool res = false;
    for (uint8_t i = 0; i < len; i++) {
        res |= set_override(i, overrides[i]);
    }
    return res;
}

bool LinuxRCInput::set_override(uint8_t channel, int16_t override) 
{
    if (override < 0) return false; /* -1: no change. */
    if (channel < 8) {
        _override[channel] = override;
        if (override != 0) {
            new_rc_input = true;
            return true;
        }
    }
    return false;
}

void LinuxRCInput::clear_overrides()
{
    for (uint8_t i = 0; i < 8; i++) {
	_override[i] = 0;
    }
}

#endif // CONFIG_HAL_BOARD
