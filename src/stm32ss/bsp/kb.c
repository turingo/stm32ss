#include "kb.h"

static int this_pressed;
static int this_up;
static int this_down;
static int this_old;

/*! Initialize the Keyboard !*/
void kb_init(void)
{
    GPIO_InitTypeDef gpio;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_0;
    gpio.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &gpio);

    this_pressed = 0;
    this_up = 0;
    this_down = 0;
    this_old = 0;
}

/*! Scan the KEYs periodically !*/
void kb_scan(void)
{
    this_pressed = 0;
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET) this_pressed |= KEY_UP;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET) this_pressed |= KEY_0;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_RESET) this_pressed |= KEY_1;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_RESET) this_pressed |= KEY_2;

    this_up = (this_pressed ^ this_old) & this_old;
    this_down = (this_pressed ^ this_old) & this_pressed;
    this_old = this_pressed;
}

/*! Get the KEYs state !*/
int kb_get(int state)
{
    if(state == KB_PRESSED) return this_pressed;
    if(state == KB_UP) return this_up;
    if(state == KB_DOWN) return this_down;
    return 0;
}
