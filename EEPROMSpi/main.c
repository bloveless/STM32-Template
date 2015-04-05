#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include "spi.h"
#include "eeprom.h"

int main(void)
{
	int i, j;

	// Enable spi clock, configure pins, configure spi unit
	// This will also enable the chip select pin
	spiInit(SPI2);

	eepromWriteEnable();

	while(1);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
	/* Infinite loop */
	/* Use GDB to find out why we're here */
	while(1);
}
#endif