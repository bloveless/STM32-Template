#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include "spi.h"

static const uint16_t speeds[] = {
	[SPI_SLOW]   = SPI_BaudRatePrescaler_64,
	[SPI_MEDIUM] = SPI_BaudRatePrescaler_8,
	[SPI_FAST]   = SPI_BaudRatePrescaler_2,
};

void spiInit(SPI_TypeDef *SPIx)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	SPI_StructInit(&SPI_InitStructure);

	// Enable the chip select pin
	csInit();

	if(SPIx == SPI2)
	{
		// Enable Peripheral Clocks for spi 2
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		// SCK   PB13  Alternate function push-pull (50MHz)
		// MISO  PB14  Input pull-up
		// MOSI  PB15  Alternate function push-pull (50MHz)
		GPIO_InitTypeDef	GPIO_InitStructure;

		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU; // input pull-up
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	else {
		return;
	}

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = speeds[SPI_SLOW];
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPIx, &SPI_InitStructure);

	SPI_Cmd(SPIx, ENABLE);
}

int spiReadWrite(SPI_TypeDef* SPIx, uint8_t *rbuf, const uint8_t *tbuf, int cnt, enum spiSpeed speed)
{
	int i;

	SPIx->CR1 = (SPIx->CR1 & ~SPI_BaudRatePrescaler_256) | speeds[speed];

	for(i = 0; i < cnt; i++)
	{
		if(tbuf)
		{
			SPI_I2S_SendData(SPIx, *tbuf++);
		}
		else {
			SPI_I2S_SendData(SPIx, 0xff);
		}

		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

		if(rbuf)
		{
			*rbuf++ = SPI_I2S_ReceiveData(SPIx);
		}
		else {
			SPI_I2S_ReceiveData(SPIx);
		}
	}

	return i;
}

int spiReadWrite16(SPI_TypeDef* SPIx, uint16_t *rbuf, const uint16_t *tbuf, int cnt, enum spiSpeed speed)
{
	int i;

	SPIx->CR1 = (SPIx->CR1 & ~SPI_BaudRatePrescaler_256) | speeds[speed];

	// temp set the data size to 16 bits
	SPI_DataSizeConfig(SPIx, SPI_DataSize_16b);

	for(i = 0; i < cnt; i++)
	{
		if(tbuf)
		{
			SPI_I2S_SendData(SPIx, *tbuf++);
		}
		else {
			SPI_I2S_SendData(SPIx, 0xff);
		}

		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

		if(rbuf)
		{
			*rbuf++ = SPI_I2S_ReceiveData(SPIx);
		}
		else {
			SPI_I2S_ReceiveData(SPIx);
		}
	}

	// set the data size back to 8 bits
	SPI_DataSizeConfig(SPIx, SPI_DataSize_8b);

	return i;
}

// Initialize the chip select pin (PC06)
void csInit()
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}