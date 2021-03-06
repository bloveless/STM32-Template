#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include "uart.h"

void Delay(uint32_t nTime);

int main(void)
{
	// Configure SysTick Timer (3)
	if(SysTick_Config(SystemCoreClock / 1000))
	{
		while (1);
	}

	uart_open(USART1, 9600, 0);

	while(1)
	{
		static int ledval = 0;

		// 1 == echo input and 0 == print hello world
		if(1)
		{
			int receivedChar = uart_getc(USART1);
			if(receivedChar)
			{
				if(receivedChar == '\r')
				{
					// returns need a little help
					uart_putc('\r', USART1);
					uart_putc('\n', USART1);
				}
				else
				{
					uart_putc(receivedChar, USART1);
					// double the response back for testing
					// uart_putc(receivedChar, USART1);
				}
			}
		}
		else
		{
			uart_putc('h', USART1);
			uart_putc('e', USART1);
			uart_putc('l', USART1);
			uart_putc('l', USART1);
			uart_putc('o', USART1);
			uart_putc(' ', USART1);
			uart_putc('w', USART1);
			uart_putc('o', USART1);
			uart_putc('r', USART1);
			uart_putc('l', USART1);
			uart_putc('d', USART1);
			uart_putc('\r', USART1);
			uart_putc('\n', USART1);

			Delay(250); // wait 250ms
		}
	}
}

// Timer code (5)
static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

void SysTick_Handler(void)
{
	if(TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
	/* Infinite loop */
	/* Use GDB to find out why we're here */
	while(1);
}
#endif