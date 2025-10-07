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

const uint32_t USART_CLOCK_SPEED = 16000000; // 16MHz
const uint32_t USART_BAUD_RATE   = 9600;

void init_serial(GPIO_TypeDef *portTx, uint32_t bitTx, GPIO_TypeDef *portRx, uint32_t bitRx)
{
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Turn on USART2

	pin_mode(portTx, bitTx, PINMODE_ALTERNATE); // enable alternative function (UART tx)
	pin_mode(portRx, bitRx, PINMODE_ALTERNATE); // enable alternative function (UART rx)
    
    pull_mode(portTx, bitTx, PULLMODE_PULLUP);
    pull_mode(portRx, bitRx, PULLMODE_PULLUP);

    // Select UART2 for ports
    volatile uint32_t* afrTx = &portTx->AFR[bitTx > 7];
    volatile uint32_t* afrRx = &portRx->AFR[bitRx > 7];

	// select uart 2 alternative function for PA2 and PA15 
    *afrTx &= ~(0b1111 << bitTx * 4);
    *afrTx |= (0b0100 << bitTx * 4);

    *afrRx &= ~(0b1111 << bitRx * 4);
    *afrRx |= (0b0100 << bitRx * 4);
	
	RCC->APB1RSTR &= ~RCC_APB1RSTR_USART2RST;

	USART2->CR1 = 0;
	USART2->CR2 = 0;
    USART2->CR3 = 0;
    USART2->BRR = USART_CLOCK_SPEED / USART_BAUD_RATE;
	
    USART2->CR3 |= USART_CR3_OVRDIS; // disable over-run errors
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE;
	USART2->CR1 |= USART_CR1_UE;
}
