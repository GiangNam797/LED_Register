#include "stm32f411xe.h"


	
	void SystemClock_Config(void);
	void GPIO_Config (void);
	void delay (uint32_t time);
	
	int main(void)
	{
		
	SystemClock_Config ();
	GPIO_Config ();
		
		while(1)
		{
		GPIOD->BSRR |= (1<<15);  						// Set the pin PD15
		delay (10000000); 
		GPIOD->BSRR |= (1 << 31);   // Reset pin PD15
		delay (10000000); 
		}
	}
	
	
	void SystemClock_Config(void)
	{
		#define PLL_M 	4
		#define PLL_N 	100
		#define PLL_P 	0  // PLLP = 2
	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. ENABLE HSE and wait for the HSE to become Ready
	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	5. Configure the MAIN PLL
	6. Enable the PLL and wait for it to become ready
	7. Select the Clock Source and wait for it to be set
	
	********************************************************/
	//	1. ENABLE HSE and wait for the HSE to become Ready
	RCC->CR |= RCC_CR_HSEON;  
	while (!(RCC->CR & RCC_CR_HSERDY));
	
	//	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS; 

	//	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_3WS;
	
	//	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	// AHB PR
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	
	
	// APB1 PR
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
	
	// APB2 PR
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
	

	//	5. Configure the MAIN PLL
	RCC->PLLCFGR = (PLL_M <<0) | (PLL_N << 6) | (PLL_P <<16) | (RCC_PLLCFGR_PLLSRC_HSE);
	
	//	6. Enable the PLL and wait for it to become ready
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));
	
	//	7. Select the Clock Source and wait for it to be set
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	}
	
	void GPIO_Config (void)
{
	/************ STEPS FOLLOWED ***********
	1. Enable the GPIO CLOCK
	2. Set the Pin as OUTPUT
	3. Configure the OUTPUT MODE
	***************************************/
	
	// 1. Enable the GPIOD CLOCK
	RCC->AHB1ENR |= (1<<3);  
	
	// 2. Set the Pin as OUTPUT
	GPIOD->MODER |= (1<<30);  // pin PD15(bits 31:30) as Output (01)
	
	// 3. Configure the OUTPUT MODE
	GPIOD->OTYPER = 0;
	GPIOD->OSPEEDR = 0;
}

void delay (uint32_t time)
{
	while (time--);
}