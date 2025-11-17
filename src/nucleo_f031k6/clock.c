#include <nucleo_f031k6/clock.h>
#include <nucleo_f031k6/common.h>

void init_nucleo_f031k6_systick(void)
{
    SysTick->LOAD = 48000;
    SysTick->CTRL = 0x7;
    SysTick->VAL = 10;

    __asm(" cpsie i ");
}

uint16_t s_nucleo_f031k6_millis = 0;
void SysTick_Handler(void)
{
    s_nucleo_f031k6_millis++;   
}

void nucleo_f031k6_delay(volatile uint16_t ms)
{
    uint16_t end_time = ms + s_nucleo_f031k6_millis;

	  while(s_nucleo_f031k6_millis != end_time)
	    __asm(" wfi "); // sleep
}

void init_nucleo_f031k6_pll(void) 
{
  // This is potentially a dangerous function as it could
  // result in a system with an invalid clock signal - result: a stuck system
  // Set the PLL up
  // First ensure PLL is disabled
  RCC->CR &= ~RCC_CR_PLLON;
  while (RCC->CR & RCC_CR_PLLRDY); // wait for PLL ready to be cleared

  // Warning here: if system clock is greater than 24MHz then wait-state(s) need
  // to be inserted into Flash memory interface

  FLASH->ACR |= FLASH_ACR_LATENCY;
  FLASH->ACR &= ~(FLASH_ACR_LATENCY1 |
                  FLASH_ACR_LATENCY2); // Clear bits 1 and 2 for latency
  // Turn on FLASH prefetch buffer
  FLASH->ACR |= FLASH_ACR_PRFTBE;
  // set PLL multiplier to 12 (yielding 48MHz)
  RCC->CFGR &= ~(RCC_CFGR_PLLMUL); // Clear PLL mul
  RCC->CFGR |= RCC_CFGR_PLLMUL_3 | RCC_CFGR_PLLMUL_1;

  // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
  RCC->CFGR |= RCC_CFGR_ADCPRE;

  // and turn the PLL back on again
  RCC->CR |= RCC_CR_PLLON;
  // set PLL as system clock source
  RCC->CFGR |= RCC_CFGR_SW_PLL;
}

void init_nucleo_f031k6_hsi16(void) 
{
  RCC->CR &= ~RCC_CR_PLLON;
  RCC->CR |= RCC_CR_HSION;
  while (0 == (RCC->CR & RCC_CR_HSIRDY)); // Wait for ready

  RCC->CFGR |= RCC_CFGR_SW_HSI;
}
