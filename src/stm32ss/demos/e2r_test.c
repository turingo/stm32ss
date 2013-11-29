#include "utils.h"
#include "kb.h"
#include "sp.h"
#include "e2r.h"

/*! EEPROM test !*/
void e2r_test(void)
{
    uint8_t datas[256];
    int kd;
    int flag;
    int i;

    kb_init();
    sp_init();
    sp_puts("EEPROM Test\r\n");
    e2r_init();

    for(i = 0; i < 256; i++) datas[i] = i;

    for(;;)
    {
        kd = kb_get(KB_DOWN);

        if(kd & KEY_0)
        {
            sp_puts("EEPROM write datas ... ");
            e2r_write(0, datas, 256);
            sp_puts("OK\r\n");
        }

        if(kd & KEY_1)
        {
            for(i = 0; i < 256; i++) datas[i] = 0;
            sp_puts("EEPROM read datas ... ");
            e2r_read(0, datas, 256);
            sp_puts("OK\r\n");

            sp_puts("EEPROM verify datas ... ");
            flag = 0;
            for(i = 0; i < 256; i++)
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
