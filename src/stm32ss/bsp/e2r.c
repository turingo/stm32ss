#include "e2r.h"

#define EEPROM_ADDR     (0xa0)
#define PAGE_SIZE       (8)

/*! Write a page to the EEPROM !*/
static void write_page(int addr, const uint8_t* datas, int len)
{
    int i;

    I2C_AcknowledgeConfig(I2C2, ENABLE);
    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)){};
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)){}; 
    I2C_Send7bitAddress(I2C2, EEPROM_ADDR, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){};  
    I2C_SendData(I2C2, addr);  
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){};
    for(i = 0; i < len; i++)
    {
        I2C_SendData(I2C2, datas[i]); 
        while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){};
    }
    I2C_GenerateSTOP(I2C2, ENABLE);

    do
    {
        I2C_GenerateSTART(I2C2, ENABLE);
        I2C_ReadRegister(I2C2, I2C_Register_SR1);
        I2C_Send7bitAddress(I2C2, EEPROM_ADDR, I2C_Direction_Transmitter);
    }while(!(I2C_ReadRegister(I2C2, I2C_Register_SR1) & 0x0002));
    I2C_ClearFlag(I2C2, I2C_FLAG_AF);
    I2C_GenerateSTOP(I2C2, ENABLE);
}

/*! Initialize the EEPROM !*/
void e2r_init(void)
{
    GPIO_InitTypeDef gpio;
	I2C_InitTypeDef i2c;
	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);  
    
    gpio.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &gpio);

    i2c.I2C_Mode = I2C_Mode_I2C;
    i2c.I2C_DutyCycle = I2C_DutyCycle_2;
    i2c.I2C_Ack = I2C_Ack_Enable;
    i2c.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    i2c.I2C_ClockSpeed = 360 * 1000;
    I2C_Cmd(I2C2, ENABLE);
    I2C_Init(I2C2, &i2c);
}

/*! Read datas from the EEPROM !*/
void e2r_read(int addr, uint8_t* datas, int len)
{
    int i;

    I2C_AcknowledgeConfig(I2C2, ENABLE);
    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)){};
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)){};
    I2C_Send7bitAddress(I2C2, EEPROM_ADDR, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){};
    I2C_SendData(I2C2, addr);  
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){};
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)){};
    I2C_Send7bitAddress(I2C2, EEPROM_ADDR, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){};
    for(i = 0; i < len; i++)
    {
        if(i == len - 1)
        {
            I2C_AcknowledgeConfig(I2C2, DISABLE);
            I2C_GenerateSTOP(I2C2, ENABLE);
        }
        while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)){}; 
        datas[i] = I2C_ReceiveData(I2C2);
    }
}

/*! Write datas to the EEPROM !*/
void e2r_write(int addr, const uint8_t* datas, int len)
{
    int wa = addr;
    int wl = 0;
    int d;

    if(wa % PAGE_SIZE)
    {
        d = PAGE_SIZE - wa % PAGE_SIZE;
        write_page(wa, &datas[wl], d);
        wa += d;
        wl += d;
    }
    
    while(wl < len)
    {
        if(len - wl < PAGE_SIZE) d = len - wl;
        else d = PAGE_SIZE;
        write_page(wa, &datas[wl], d);
        wa += d;
        wl += d;    
    }    
}
