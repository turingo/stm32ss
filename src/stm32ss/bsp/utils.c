#include "utils.h"

/*! Delay some microseconds !*/
void delayus(int us)
{
    volatile int i;
    while(us--)
    {
        for(i = 0; i < 12; i++){};        
    }
}

/*! Delay some milliseconds !*/
void delayms(int ms)
{
    volatile int i;
    while(ms--)
    {
        for(i = 0; i < 12000; i++){};        
    }
}
