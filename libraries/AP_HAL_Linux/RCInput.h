
#ifndef __AP_HAL_LINUX_RCINPUT_H__
#define __AP_HAL_LINUX_RCINPUT_H__

#include <AP_HAL_Linux.h>
#define PRU0_SHARED_BASE     0x4a311000
#define TICK_PER_US 200
#define TICK_PER_S 200000000


class Linux::LinuxRCInput : public AP_HAL::RCInput {
public:
    LinuxRCInput();
    void init(void* machtnichts);
    bool new_input();
    uint8_t num_channels();
    uint16_t read(uint8_t ch);
    uint8_t read(uint16_t* periods, uint8_t len);

    bool set_overrides(int16_t *overrides, uint8_t len);
    bool set_override(uint8_t channel, int16_t override);
    void clear_overrides();

 private:
    bool new_rc_input;
    struct rcinput {
      uint32_t pinToMonitor;
      uint32_t header;
      uint32_t tail;
      struct values {
	uint32_t value;
	uint32_t timestamp;
      } vals[100];
    }*rcin;
    /* override state */
    uint16_t _override[8];
};

#endif // __AP_HAL_LINUX_RCINPUT_H__
