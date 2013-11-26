#include "utils.h"
#include "leds.h"

/*! Marquee demo !*/
void marquee(void)
{
    leds_init();
    leds_ctrl(LEDS_RED, LEDS_TURN_ON);

    for(;;)
    {
        delayms(300);
        leds_ctrl(LEDS_RED, LEDS_TOGGLE);
        leds_ctrl(LEDS_GREEN, LEDS_TOGGLE);
    }
}
