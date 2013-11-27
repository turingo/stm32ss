#ifndef __BCQ_H__
#define __BCQ_H__

#include "stm32f10x.h"

/*! The Byte Stream Circular Queue type !*/
typedef struct
{
    uint8_t* elems;
    int size;
    int front;
    int rear;
    int count;
}bcq_t;

/*! The Byte Stream Circular Queue APIs !*/
void bcq_init(bcq_t* this, uint8_t* elems, int size);
int bcq_empty(const bcq_t* this);
int bcq_full(const bcq_t* this);
void bcq_push(bcq_t* this, uint8_t elem);
uint8_t bcq_pop(bcq_t* this);

#endif
