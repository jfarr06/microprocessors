/**
 * @file: nucleo_f031k6/clock.h
 * @desc: Clock and timing functions - PLL, SysTick, and delays
 * @auth: James Farrelly (C24402114)
 */

#ifndef NUCLEO_F031K6_CLOCK_H_
#define NUCLEO_F031K6_CLOCK_H_

#include <stdint.h>

/**
 * The current millis systick value.
 */
extern uint16_t s_nucleo_f031k6_millis;

/**
 * Enables SysTick
 */
void init_nucleo_f031k6_systick(void);

/**
 * Initializes PLL clock, and sets it as current clock.
 */
void init_nucleo_f031k6_pll(void);

/**
 * Disable PLL clock, enable HSI16 and set current clock.
 */
void init_nucleo_f031k6_hsi16(void);

/**
 * Delay for a specified number of milliseconds.
 * 
 * @param ms Number of milliseconds to delay
 */
void nucleo_f031k6_delay(volatile uint16_t ms);

/**
 * Get the current millisecond count since system start.
 * 
 * @return Current millisecond count
 */
static inline uint16_t nucleo_f031k6_millis(void)
{
    return s_nucleo_f031k6_millis;
}

#endif
