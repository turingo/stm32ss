#include "bcq.h"

/*! Initialize the Queue !*/
void bcq_init(bcq_t* this, uint8_t* elems, int size)
{
    this->elems = elems;
    this->size = size;
    this->front = 0;
    this->rear = 0;
    this->count = 0;
}

/*! Check if the Queue empty !*/
int bcq_empty(const bcq_t* this)
{
    return this->count == 0;
}

/*! Check if the Queue full !*/
int bcq_full(const bcq_t* this)
{
    return this->count == this->size;
}

/*! Get the Queue element count !*/
int bcq_count(const bcq_t* this)
{
    return this->count;
}

/*! Push a element to the Queue !*/
void bcq_push(bcq_t* this, uint8_t elem)
{
    if(bcq_full(this)) return;

    this->elems[this->rear] = elem;
    if(++this->rear >= this->size) this->rear = 0;
    this->count++;
}

/*! Pop a element from the Queue !*/
uint8_t bcq_pop(bcq_t* this)
{
    uint8_t elem = 0;
    if(bcq_empty(this)) return elem;

    elem = this->elems[this->front];
    if(++this->front >= this->size) this->front = 0;
    this->count--;
    return elem;
}
