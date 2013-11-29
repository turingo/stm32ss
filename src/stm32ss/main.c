#include "stm32f10x.h"
#include "demos.h"

/*! C entry !*/
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //marquee();
    //two_tigers();
    //kb_test();
    //sp_test();
    e2r_test();

    return 0;
}
