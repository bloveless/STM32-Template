#include <stm32f10x.h>

void Delay(uint32_t nTime);

int main(void)
{
	// Enable the clock for port A and C
	RCC->APB2ENR |= (RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN);	//enable portc

	// SET PIN A0 to Input mode (MODE 00) and Floating input (CNF 01)
	GPIOA->CRL = 0x44444444;

	// SET PIN C9 to Output @ 2MHz (MODE 10) and general purpose output push-pull (CNF 01)
	// GPIOC->CRH = 0x66666666;
	GPIOC->CRH = 0x44444422;

	while(1)
	{
		// Read out the input from GPIO A0
		if(GPIOA->IDR & GPIO_IDR_IDR0)
		{
			// If it was not 0 then turn on GPIO C9 and GPIO C8 and all others off
			GPIOC->ODR = 0 | GPIO_ODR_ODR9 | GPIO_ODR_ODR8;
		}
		else
		{
			// If it was 0 then turn on GPIO C8 and all others off
			GPIOC->ODR = 0 | GPIO_ODR_ODR8;
		}
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