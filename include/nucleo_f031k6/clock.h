#ifndef NUCLEO_F031K6_CLOCK_H_
#define NUCLEO_F031K6_CLOCK_H_

/**
 * Enables SysTick
 */
void init_systick();

/**
 * Initializes PLL clock, and sets it as current clock.
 */
void init_pll();

/**
 * Disable PLL clock, enable HSI16 and set current clock.
 */
void init_hsi16();

#endif
