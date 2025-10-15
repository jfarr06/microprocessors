#include "nucleo_l031k6_defines.h"
#include <stm32l031xx.h>
#include <nucleo_l031k6_common.h>

void pin_mode(GPIO_TypeDef* port, uint32_t bit, uint32_t mode)
{
    mode <<= bit * 2;

    port->MODER &= ~(3u << (bit * 2));
    port->MODER |= mode;
}

void pull_mode(GPIO_TypeDef* port, uint32_t bit, uint32_t mode)
{
    mode <<= bit * 2;

    port->PUPDR &= ~(3u << (bit * 2));
    port->PUPDR |= mode;
}

void toggle_odr_bit(GPIO_TypeDef* port, uint32_t bit, bool status)
{
    if (status)
        port->ODR |= bit;
    else
        port->ODR &= ~bit;
}

bool get_input_status(GPIO_TypeDef* port, uint32_t bit, uint8_t mode)
{
    if (mode > INPUTMODE_PULLUP) mode = INPUTMODE_PULLUP;

    return (1^mode) == (port->IDR & bit);
}

uint32_t read_adc()
{
    ADC1->CR |= ADC_CR_ADSTART;
    while (ADC1->CR & ADC_CR_ADSTART); // Wait for end
    
    return ADC1->DR;
}

void play_sound(uint32_t frequency)
{
    if (frequency == 0) TIM2->CR1 &= ~TIM_CR1_CEN; // disable the counter

	TIM2->ARR = 0x3d0900u / frequency; 
	TIM2->CCR4= TIM2->ARR / 2;	

	TIM2->CNT = 0; // set the count to zero initially
	TIM2->CR1 |= TIM_CR1_CEN; // and enable the counter 
}

volatile uint32_t sys_millis = 0;

void SysTick_Handler()
{
    sys_millis++;
}

void sys_delay(uint32_t dly)
{
    uint32_t end_time = dly + sys_millis;

	while(sys_millis != end_time)
		__asm(" wfi "); // sleep
}

void init_systick()
{
    SysTick->LOAD = 48000;
    SysTick->CTRL = 0x7; // Window watchdog
    SysTick->VAL = 0xA;

    __asm(" cpsie i "); // Enable interrupts.
}

void init_hsi16()
{
    RCC->CR &= ~RCC_CR_PLLON;
    RCC->CR |= RCC_CR_HSION;
    while (0 == (RCC->CR & RCC_CR_HSIRDY)); // Wait for ready

    RCC->CFGR |= RCC_CFGR_SW_HSI;
}

void init_adc()
{
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
    ADC->CCR |= ADC_CCR_VLCDEN; // Ref volt

    ADC1->CR = ADC_CR_ADCAL; // Calibrate ADC
    while(ADC1->CR & ADC_CR_ADCAL); // Wait for calibration finish.

    ADC1->CR |= ADC_CR_ADEN;
}

void init_sound(GPIO_TypeDef* port, uint32_t bit)
{
    pin_mode(port, bit, PINMODE_ALTERNATE); // select alternative function for PB1

    port->AFR[0] &= ~((0b1111) << bit*4); // zero out alternate function bits
    port->AFR[0] |= ((0b0101) << bit*4); // set required alternate function bits (AF5)
    
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // enable Timer 2
	TIM2->CCMR2 = TIM_CCMR2_OC4M_2 + TIM_CCMR2_OC4M_1;
	TIM2->CCER |= TIM_CCER_CC4E;

    TIM2->CR1 = 0;
	TIM2->PSC = 3;
	TIM2->ARR = 1UL;
	TIM2->CCR4 = 0;	
	TIM2->CNT = 0;
}
