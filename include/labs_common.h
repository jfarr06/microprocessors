#ifndef LABS_COMMON_H_
#define LABS_COMMON_H_

#include <stdint.h>
#include <stdbool.h>

// Commonly used pinouts within labs for lights
#define GREEN_LIGHT  GPIO_ODR_OD0 // PA0
#define YELLOW_LIGHT GPIO_ODR_OD1 // PA1
#define RED_LIGHT    GPIO_ODR_OD2 // PA2

#define GREEN_LIGHT_PINOUT  GPIO_ODR_OD0_Pos // PA0
#define YELLOW_LIGHT_PINOUT GPIO_ODR_OD1_Pos // PA1
#define RED_LIGHT_PINOUT    GPIO_ODR_OD2_Pos // PA2

/**
 * Creates artificial delay.
 *
 * @param dly The delay amount.
 */
void delay(volatile uint32_t dly);

//==================== LIGHTS

/**
 * Initialises the common lights used in labs.
 */
void init_common_lights();

/**
 * Toggles the green light
 *
 * @param status True or false value of the green light.
 */
void toggle_green(bool status);

/**
* Toggles the yellow light
*
* @param status True or false value of the yellow light.
*/
void toggle_yellow(bool status);

/**
 * Toggles the green light
 *
 * @param status True or false value of the red light.
 */
void toggle_red(bool status);

//=================== RANDOM

/**
 * Initializes the pseudo random number generator.
 */
void init_rand();

/**
 * Generates a psuedo random number between hi and lo.
 *
 * @param lo The lower bound.
 * @param hi The higher bound.
 */ 
uint32_t rand(uint32_t lo, uint32_t hi);

/**
 * Writes a character to the serial output.
 *
 * @param c The char
 */
void eputchar(char c);

/**
 * Waits for a character from the serial input.
 */
char egetchar();

/**
 * Puts a string to the serial output.
 *
 * @param str The string to output.
 */
void eputs(char* str);

#endif
