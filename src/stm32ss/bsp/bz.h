#ifndef __BZ_H__
#define __BZ_H__

#include "stm32f10x.h"

/*! The Buzzer commands !*/
#define BZ_TURN_OFF     (0)
#define BZ_TURN_ON      (1)
#define BZ_TOGGLE       (2)

/*! The Buzzer APIs !*/
void bz_init(void);
void bz_ctrl(int cmd);

#endif
