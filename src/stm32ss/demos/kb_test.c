#include "utils.h"
#include "leds.h"
#include "bz.h"
#include "kb.h"

/*! Keyboard test !*/
void kb_test(void)
{
    int kd;

    leds_init();
    bz_init();
    kb_init();

    for(;;)
    {
        delayms(20);
        kb_scan();
        kd = kb_get(KB_DOWN);        

        if(kd & KEY_UP) bz_ctrl(BZ_TOGGLE);
        if(kd & KEY_0)
        {
            leds_ctrl(LEDS_RED, LEDS_TOGGLE);
            leds_ctrl(LEDS_GREEN, LEDS_TOGGLE);
        }
        if(kd & KEY_1) leds_ctrl(LEDS_GREEN, LEDS_TOGGLE);
        if(kd & KEY_2) leds_ctrl(LEDS_RED, LEDS_TOGGLE);         
    }
}
