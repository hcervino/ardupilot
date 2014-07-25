/*
 * testpru
 *
 */

#define PRU0
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <strings.h>

#include "linux_types.h"
#include "pru_defs.h"
#include "prucomm.h"




static inline u32 read_PIEP_COUNT(void)
{
	return PIEP_COUNT;

}

struct rcinput {
  u32 pinToMonitor;
  u32 header;
  u32 tail;
}rcin;

struct values {
  u32 highlow;
  u32 timestamp;
}val;

int main(int argc, char *argv[])
{

	u32 cnt, next;
	/* enable OCP master port */
	PRUCFG_SYSCFG &= ~SYSCFG_STANDBY_INIT;
	PRUCFG_SYSCFG = (PRUCFG_SYSCFG &
			~(SYSCFG_IDLE_MODE_M | SYSCFG_STANDBY_MODE_M)) |
			SYSCFG_IDLE_MODE_NO | SYSCFG_STANDBY_MODE_NO;

	/* our PRU wins arbitration */
	PRUCFG_SPP |=  SPP_PRU0_PAD_HP_EN;
	

	/* configure timer */
	PIEP_GLOBAL_CFG = GLOBAL_CFG_DEFAULT_INC(1) |
			  GLOBAL_CFG_CMP_INC(1);
	PIEP_CMP_STATUS = CMD_STATUS_CMP_HIT(1); /* clear the interrupt */
        PIEP_CMP_CMP1   = 0x0;
	PIEP_CMP_CFG |= CMP_CFG_CMP_EN(1);
        PIEP_GLOBAL_CFG |= GLOBAL_CFG_CNT_ENABLE;

	while(1) {
	  cnt = read_PIEP_COUNT();
	  val.timestamp = cnt;
	  val.highlow = pru_read_other_reg(15);
	  
	}
		/* loop while nothing changes */
		do {
		  cnt = read_PIEP_COUNT();
		} while (((next - cnt) & (1U << 31)) == 0);
}
