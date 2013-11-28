#ifndef __SP_H__
#define __SP_H__

#include "stm32f10x.h"

/*! The Serial Port APIs !*/
void sp_init(void);
int sp_empty(void);
int sp_full(void);
int sp_count(void);
char sp_getc(void);
void sp_putc(char ch);
void sp_puts(const char* str);
void sp_read(uint8_t* datas, int len);
void sp_write(const uint8_t* datas, int len);

#endif
