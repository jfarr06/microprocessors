/**
 * @file: nucleo_f031k6/adc.c
 * @desc: ADC (Analog-to-Digital Converter) implementation for STM32F031K6
 * @auth: James Farrelly (C24402114)
 */

#include <nucleo_f031k6/adc.h>
#include <nucleo_f031k6/common.h>
#include <debug.h>

void init_nucleo_f031k6_adc(void)
{
    DBG_INFO("Initializing ADC with calibration");
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
    ADC->CCR |= ADC_CCR_VLCDEN; // Ref volt

    ADC1->CR = ADC_CR_ADCAL; // Calibrate ADC
    while(ADC1->CR & ADC_CR_ADCAL); // Wait for calibration finish.

    ADC1->CR |= ADC_CR_ADEN;
    DBG_INFO("ADC initialized and calibrated");
}

uint32_t read_nucleo_f031k6_adc(void)
{
    ADC1->CR |= ADC_CR_ADSTART;
    while (ADC1->CR & ADC_CR_ADSTART); // Wait for end
    
    return ADC1->DR;
}
