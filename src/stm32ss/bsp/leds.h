#ifndef __LEDS_H__
#define __LEDS_H__

#include "stm32f10x.h"

/*! The LEDs !*/
#define LEDS_RED        (0)
#define LEDS_GREEN      (1)

/*! The LED commands !*/
#define LEDS_TURN_OFF       (0)
#define LEDS_TURN_ON        (1)
#define LEDS_TOGGLE         (2)

/*! The LED APIs !*/
void leds_init(void);
void leds_ctrl(int led, int cmd);

#endif
