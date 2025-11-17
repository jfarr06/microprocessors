#ifndef NUCLEO_F031K6_COMMON_H_
#define NUCLEO_F031K6_COMMON_H_

#include <stdint.h>
#include <stdbool.h>

#include <stm32f031x6.h>
#include <nucleo_f031k6/defines.h>

//========== FUNCS

/**
 * Sets the pinmode for the specified bit on the specified port.
 *
 * @param port The GPIO port to use.
 * @param bit The specific pin of the GPIO port to set the mode for.
 * @param mode The mode to choose.
 */
void set_nucleo_f031k6_pin_mode(GPIO_TypeDef* port, uint8_t bit, uint32_t mode);

/**
 * Enables the pull mode for the specific pin on the GPIO port.
 *
 * @param port The GPIO port.
 * @param bit The GPIO port pin.
 * @param mode The specific mode for the pull resistor
 */
void set_nucleo_f031k6_pull_mode(GPIO_TypeDef* port, uint8_t bit, uint32_t mode);

/**
 * Toggles the value of specified bit in the specified IO register
 *
 * @param ioReg The specific GPIO port.
 * @param bit The specific pin on the GPIO port.
 * @param status Either 1 or 0.
 */
void toggle_nucleo_f031k6_odr_bit(GPIO_TypeDef* port, uint32_t bit, bool status);

/**
 * Gets the status of an IDR register for the specified GPIO port.
 *
 * @param port The specific GPIO port.
 * @param bit The specific pin on the GPIO port.
 */
bool get_nucleo_f031k6_idr_bit(GPIO_TypeDef* port, uint32_t bit, uint8_t mode);

/**
 * Delay for the specified amount of milliseconds.
 */
void nucleo_f031k6_delay(volatile uint16_t ms);

#endif
