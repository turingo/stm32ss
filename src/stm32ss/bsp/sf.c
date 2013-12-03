#include "sf.h"

/*! The instructions of the SPI FLASH !*/
#define INS_JEDEC_ID            (0x9f)
#define INS_READ_STATUS         (0x05)
#define INS_WRITE_ENABLE        (0x06)
#define INS_WRITE_DISABLE       (0x04)
#define INS_CHIP_ERASE          (0xc7)
#define INS_SECTOR_ERASE        (0x20)
#define INS_BLOCK_ERASE         (0xd8)
#define INS_READ_DATA           (0x03)
#define INS_PAGE_PROGRAM        (0x02)

/*! The status bits of the SPI FLASH !*/
#define STATUS_BUSY     (1 << 0)
#define STATUS_WEL      (1 << 1)
#define STATUS_BP0      (1 << 2)
#define STATUS_BP1      (1 << 3)
#define STATUS_BP2      (1 << 4)
#define STATUS_TB       (1 << 5)
#define STATUS_SEC      (1 << 6)
#define STATUS_SPR0     (1 << 7)

/*! Enable to access the SPI FLASH !*/
#define this_begin()    GPIO_ResetBits(GPIOB, GPIO_Pin_12)

/*! Disable to access the SPI FLASH !*/
#define this_end()      GPIO_SetBits(GPIOB, GPIO_Pin_12)

/*! Send and receive a byte for the SPI FLASH !*/
static uint8_t this_byte(uint8_t data)
{
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){};
    SPI_I2S_SendData(SPI2, data);
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};
    return SPI_I2S_ReceiveData(SPI2);
}

/*! Get the status of the SPI FLASH !*/
static int this_status(void)
{
    int status;

    this_begin();
    this_byte(INS_READ_STATUS);
    status = this_byte(0xff);
    this_end();

    return status;
}

/*! Wait for the SPI FLASH idle !*/
#define this_wait() while((this_status() & STATUS_BUSY)){}

/*! Enable to write the SPI FLASH !*/
#define this_write_enable()         \
    this_begin();                   \
    this_byte(INS_WRITE_ENABLE);    \
    this_end()

/*! Disable to write the SPI FLASH !*/
#define this_write_disable()        \
    this_begin();                   \
    this_byte(INS_WRITE_DISABLE);   \
    this_end()

/*! Write a page to the SPI FLASH !*/
static void this_write_page(int addr, const uint8_t* datas, int len)
{
    int i;

    this_begin();
    this_byte(INS_PAGE_PROGRAM);
    this_byte((uint8_t)(addr >> 16));
    this_byte((uint8_t)(addr >> 8));
    this_byte((uint8_t)addr);
    for(i = 0; i < len; i++)
        this_byte(datas[i]);
    this_end();

    this_wait();
}

/*! Initialize the SPI FLASH !*/
int sf_init(void)
{
    GPIO_InitTypeDef gpio;
    SPI_InitTypeDef spi;
    int mid = 0;
	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_12;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);
    GPIO_SetBits(GPIOB, GPIO_Pin_12);

    gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &gpio);

    spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi.SPI_Mode = SPI_Mode_Master;
    spi.SPI_DataSize = SPI_DataSize_8b;
    spi.SPI_CPOL = SPI_CPOL_High;
    spi.SPI_CPHA = SPI_CPHA_2Edge;
    spi.SPI_NSS = SPI_NSS_Soft;
    spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    spi.SPI_FirstBit = SPI_FirstBit_MSB;
    spi.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &spi);
    SPI_Cmd(SPI2, ENABLE); 

    this_write_disable();
    /** Check manufacturer ID of the SPI FLASH */
    this_begin();
    this_byte(INS_JEDEC_ID);
    mid |= this_byte(0xff) << 16;
    mid |= this_byte(0xff) << 8;
    mid |= this_byte(0xff);
    this_end();

    return ((mid >> 16) == 0xef) ? 0 : -1;
}

/*! Erase the whole SPI FLASH !*/
void sf_erase(void)
{
    this_write_enable();
    this_begin();
    this_byte(INS_CHIP_ERASE);
    this_end();

    this_wait();
    this_write_disable();
}

/*! Erase a sector of the SPI FLASH !*/
void sf_erase_sector(int addr)
{
    this_write_enable();
    this_begin();
    this_byte(INS_SECTOR_ERASE);
    this_byte((uint8_t)(addr >> 16));
    this_byte((uint8_t)(addr >> 8));
    this_byte((uint8_t)addr);
    this_end();

    this_wait();
    this_write_disable();
}

/*! Erase a block of the SPI FLASH !*/
void sf_erase_block(int addr)
{
    this_write_enable();
    this_begin();
    this_byte(INS_BLOCK_ERASE);
    this_byte((uint8_t)(addr >> 16));
    this_byte((uint8_t)(addr >> 8));
    this_byte((uint8_t)addr);
    this_end();

    this_wait();
    this_write_disable();
}

/*! Read datas from the SPI FLASH !*/
void sf_read(int addr, uint8_t* datas, int len)
{
    int i;

    this_begin();
    this_byte(INS_READ_DATA);
    this_byte((uint8_t)(addr >> 16));
    this_byte((uint8_t)(addr >> 8));
    this_byte((uint8_t)addr);
    for(i = 0; i < len; i++)
        datas[i] = this_byte(0xff);
    this_end();
}

/*! Write datas to the SPI FLASH !*/
void sf_write(int addr, const uint8_t* datas, int len)
{
    uint16_t fb_len;
    uint16_t pages;
    uint16_t lb_len;    
    uint8_t* p = (uint8_t*)datas;
    int offset = addr;
    int i;

    fb_len = 256 - (addr & 0xff);
    if(fb_len < len)
    {
        pages = (len - fb_len) >> 8;
        lb_len = (len - fb_len) & 0xff;
    }
    else
    {
        fb_len = len;
        pages = 0;
        lb_len = 0;
    }

    this_write_enable();

    /** Write the first page to the SPI FLASH */
    if(fb_len > 0)
    {
        this_write_page(offset, p, fb_len);
        offset += fb_len;
        p += fb_len;
    }

    for(i = 0; i < pages; i++)
    {
        this_write_page(offset, p, 256);
        offset += 256;
        p += 256;
    }

    /** Write the last page to the SPI FLASH */
    if(lb_len > 0)
    {
        this_write_page(offset, p, lb_len);
    }

    this_write_disable();
}


