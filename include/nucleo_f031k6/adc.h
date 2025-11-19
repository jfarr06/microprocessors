/**
 * @file: nucleo_f031k6/adc.h
 * @desc: ADC (Analog-to-Digital Converter) interface for STM32F031K6
 * @auth: James Farrelly (C24402114)
 */

#ifndef NUCLEO_F031K6_ADC_H_
#define NUCLEO_F031K6_ADC_H_

#include <stdint.h>

/**
 * Initializes ADC
 */
void init_nucleo_f031k6_adc(void);

/**
 * Sends START command to ADC and reads the output of it.
 */
uint32_t read_nucleo_f031k6_adc(void);

#endif // !NUCLEO_F031K6_ADC_H_
