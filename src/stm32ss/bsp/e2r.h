#ifndef __E2R_H__
#define __E2R_H__

#include "stm32f10x.h"

/*! The EEPROM APIs !*/
void e2r_init(void);
void e2r_read(int addr, uint8_t* datas, int len);
void e2r_write(int addr, const uint8_t* datas, int len);

#endif
