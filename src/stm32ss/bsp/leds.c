#include "leds.h"

/*! Initialize the LEDs !*/
void leds_init(void)
{
    GPIO_InitTypeDef gpio;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_5;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);
    GPIO_SetBits(GPIOB, GPIO_Pin_5);

    gpio.GPIO_Pin = GPIO_Pin_5;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &gpio);
    GPIO_SetBits(GPIOE, GPIO_Pin_5);
}

/*! Control the LEDs by commands !*/
void leds_ctrl(int led, int cmd)
{
    switch(cmd)
    {
    case LEDS_TURN_OFF:
        if(led == LEDS_RED)
            GPIO_SetBits(GPIOB, GPIO_Pin_5);
        else
            GPIO_SetBits(GPIOE, GPIO_Pin_5);
        break;
    case LEDS_TURN_ON:
        if(led == LEDS_RED)
            GPIO_ResetBits(GPIOB, GPIO_Pin_5);
        else
            GPIO_ResetBits(GPIOE, GPIO_Pin_5);
        break;
    case LEDS_TOGGLE:
        if(led == LEDS_RED)
        {
            if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5) == Bit_RESET)
                GPIO_SetBits(GPIOB, GPIO_Pin_5);
            else
                GPIO_ResetBits(GPIOB, GPIO_Pin_5);
        }
        else
        {
            if(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5) == Bit_RESET)
                GPIO_SetBits(GPIOE, GPIO_Pin_5);
            else
                GPIO_ResetBits(GPIOE, GPIO_Pin_5);
        }
        break;
    default:
        break;
    }
}
