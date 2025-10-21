#ifndef NUCLEO_F031K6_ADC_H_
#define NUCLEO_F031K6_ADC_H_

#include <stdint.h>

/**
 * Initializes ADC
 */
void init_adc();

/**
 * Sends START command to ADC and reads the output of it.
 */
uint32_t read_adc();

#endif // !NUCLEO_F031K6_ADC_H_
