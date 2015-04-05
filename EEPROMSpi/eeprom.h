#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include "spi.h"

#define EEPROM_PORT GPIOC
#define EEPROM_CS GPIO_Pin_6
#define EEPROM_SPI SPI2
#define EEPROM_SPEED SPI_BaudRatePrescaler_64
#define WIP(x) ((x) & 1)

enum eepromCMD {
	cmdREAD =  0x03,
	cmdWRITE = 0x02,
	cmdWREN =  0x06,
	cmdWRDI =  0x04,
	cmdRDSR =  0x05,
	cmdWRSR =  0x01,
};

uint8_t eepromReadStatus();
void eepromWriteEnable();