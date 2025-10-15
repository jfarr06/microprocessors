#ifndef NUCLEO_L031K6_COMMON_H_
#define NUCLEO_L031K6_COMMON_H_

#include <stdint.h>
#include <stdbool.h>

#include <stm32l031xx.h>
#include <nucleo_l031k6_defines.h>

//========== FUNCS

/**
 * Sets the pinmode for the specified bit on the specified port.
 *
 * @param port The GPIO port to use.
 * @param bit The specific pin of the GPIO port to set the mode for.
 * @param mode The mode to choose.
 */
void pin_mode(GPIO_TypeDef* port, uint32_t bit, uint32_t mode);

/**
 * Enables the pull mode for the specific pin on the GPIO port.
 *
 * @param port The GPIO port.
 * @param bit The GPIO port pin.
 * @param mode The specific mode for the pull resistor
 */
void pull_mode(GPIO_TypeDef* port, uint32_t bit, uint32_t mode);

/**
 * Toggles the value of specified bit in the specified IO register
 *
 * @param ioReg The specific GPIO port.
 * @param bit The specific pin on the GPIO port.
 * @param status Either 1 or 0.
 */
void toggle_odr_bit(GPIO_TypeDef* port, uint32_t bit, bool status);

/**
 * Gets the status of an IDR register for the specified GPIO port.
 *
 * @param port The specific GPIO port.
 * @param bit The specific pin on the GPIO port.
 */
bool get_input_status(GPIO_TypeDef* port, uint32_t bit, uint8_t mode);

/**
 * Plays a tone at the specified frequency
 *
 * @param frequency The frequency.
 */
void play_sound(uint32_t frequency);

/**
 * Delays for the specified amount of time in ms using sys-tick
 *
 * @param delay The time in milliseconds.
 */
void sys_delay(uint32_t delay);

/**
 * Sends START command to ADC and reads the output of it.
 */
uint32_t read_adc();

/**
 * Enables SysTick
 */
void init_systick();

/**
 * Disable PLL clock, enable HSI16 and set current clock.
 */
void init_hsi16();

/**
 * Initializes ADC
 */
void init_adc();

/**
 * Initializes sound.
 *
 * @param port The GPIO port to use for the alternate function
 * @param bit The GPIO port bit to use for the alternate function
 */
void init_sound(GPIO_TypeDef* port, uint32_t bit);

#endif
