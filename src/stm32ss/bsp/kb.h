#ifndef __KB_H__
#define __KB_H__

#include "stm32f10x.h"

/*! The KEYs !*/
#define KEY_UP      (1 << 0)
#define KEY_0       (1 << 1)
#define KEY_1       (1 << 2)
#define KEY_2       (1 << 3)

/*! The KEY states !*/
#define KB_PRESSED  (0)
#define KB_UP       (1)
#define KB_DOWN     (2)

/*! The Keyboard APIs !*/
void kb_init(void);
void kb_scan(void);
int kb_get(int state);

#endif
