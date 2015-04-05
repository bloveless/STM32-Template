#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include "spi.h"
#include "eeprom.h"

uint8_t eepromReadStatus()
{
	uint8_t cmd[] = {cmdRDSR, 0xff};
	uint8_t res[2];

	GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 0);
	spiReadWrite(EEPROM_SPI, res, cmd, 2, EEPROM_SPEED);
	GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 1);

	return res[1];
}

void eepromWriteEnable()
{
	uint8_t cmd = cmdWREN;

	while(WIP(eepromReadStatus()));

	GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 0);
	spiReadWrite(EEPROM_SPI, 0, &cmd, 1, EEPROM_SPEED);
	GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 1);
}