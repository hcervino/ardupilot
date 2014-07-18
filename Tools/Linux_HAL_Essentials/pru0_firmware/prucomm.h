/*
 * prucomm.h - structure definitions for communication
 *
 */
#ifndef PRUCOMM_H
#define PRUCOMM_H

#include "pru_defs.h"
struct cxt {
        u32 cnt;
        u32 next;
        u32 enmask;
        u32 stmask;
        u32 setmsk;
        u32 clrmsk;
        u32 deltamin;
        u32 *next_hi_lo;
};


/* the command is at the start of shared DPRAM */
#define PWM_CMD		((volatile struct pwm_cmd *)DPRAM_SHARED)

#endif
