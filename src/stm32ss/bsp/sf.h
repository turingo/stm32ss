#ifndef __SF_H__
#define __SF_H__

#include "stm32f10x.h"

/*! The SPI FLASH Driver APIs !*/
int sf_init(void);
void sf_erase(void);
void sf_erase_sector(int addr);
void sf_erase_block(int addr);
void sf_read(int addr, uint8_t* datas, int len);
void sf_write(int addr, const uint8_t* datas, int len);

#endif
