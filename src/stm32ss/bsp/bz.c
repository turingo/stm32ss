#include "bz.h"

/*! Initialize the Buzzer !*/
void bz_init(void)
{
    GPIO_InitTypeDef gpio;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_8;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);
    GPIO_ResetBits(GPIOB, GPIO_Pin_8);
}

/*! Control the Buzzer by commands !*/
void bz_ctrl(int cmd)
{
    switch(cmd)
    {
    case BZ_TURN_OFF:
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
        break;
    case BZ_TURN_ON:
        GPIO_SetBits(GPIOB, GPIO_Pin_8);
        break;
    case BZ_TOGGLE:
        if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8) == Bit_RESET)
            GPIO_SetBits(GPIOB, GPIO_Pin_8);
        else
            GPIO_ResetBits(GPIOB, GPIO_Pin_8);
        break;
    default:
        break;
    }
}
