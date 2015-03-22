#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

void Delay(uint32_t nTime);

int main(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	// Enable Peripheral Clocks (1)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// Configure Pins (2)
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Setup pin A0 as inputload
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Configure SysTick Timer (3)
	if(SysTick_Config(SystemCoreClock / 1000))
	{
		while (1);
	}

	while(1)
	{
		static int ledval = 0;
		static int loopCount = 0;
		static uint8_t buttonStatus = 0;

		// Since we want to monitor the input button as well we will run this function only
		// once every 250 calls so that the input button can be read much more frequently
		if((loopCount % 250) == 0)
		{
			// toggle led PC9 (4)
			GPIO_WriteBit(GPIOC, GPIO_Pin_9, (ledval)? Bit_SET : Bit_RESET);
			ledval = 1 - ledval;	
		}

		buttonStatus = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
		if(buttonStatus)
		{
			GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
		}
		else
		{
			GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
		}

		loopCount++;
		Delay(1); // wait 250ms
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