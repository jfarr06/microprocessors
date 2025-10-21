#include <nucleo_f031k6/adc.h>
#include <nucleo_f031k6/common.h>

void init_adc()
{
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
    ADC->CCR |= ADC_CCR_VLCDEN; // Ref volt

    ADC1->CR = ADC_CR_ADCAL; // Calibrate ADC
    while(ADC1->CR & ADC_CR_ADCAL); // Wait for calibration finish.

    ADC1->CR |= ADC_CR_ADEN;
}

uint32_t read_adc()
{
    ADC1->CR |= ADC_CR_ADSTART;
    while (ADC1->CR & ADC_CR_ADSTART); // Wait for end
    
    return ADC1->DR;
}
