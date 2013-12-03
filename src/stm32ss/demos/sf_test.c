#include "utils.h"
#include "kb.h"
#include "sp.h"
#include "sf.h"

/*! SPI FLASH test !*/
void sf_test(void)
{
    uint8_t datas[6502];
    int kd;
    int flag;
    int i;

    kb_init();
    sp_init();
    sp_puts("SPI FLASH Test\r\n");
    sf_init();

    for(i = 0; i < 6502; i++) datas[i] = i;

    for(;;)
    {
		kb_scan();
        kd = kb_get(KB_DOWN);

        if(kd & KEY_0)
        {
            sp_puts("SPI FLASH write datas ... ");
            sf_write(0x8051, datas, 6502);
            sp_puts("OK\r\n");
        }

        if(kd & KEY_1)
        {
            for(i = 0; i < 6502; i++) datas[i] = 0;
            sp_puts("SPI FLASH read datas ... ");
            sf_read(0x8051, datas, 6502);
            sp_puts("OK\r\n");

            sp_puts("SPI FLASH verify datas ... ");
            flag = 0;
            for(i = 0; i < 6502; i++)
            {
                if(datas[i] != i)
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)   sp_puts("SUCCESS\r\n");
            else sp_puts("FAILED\r\n");                
        }
    }
}
