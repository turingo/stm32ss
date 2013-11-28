#include "utils.h"
#include "sp.h"

/*! Serial Port test !*/
void sp_test(void)
{
    sp_init();
    sp_puts("Serial Port Test\r\n");

    for(;;)
    {
        if(!sp_empty()) sp_putc(sp_getc());
    }
}
