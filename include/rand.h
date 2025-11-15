#ifndef RAND_H_
#define RAND_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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
uint32_t sys_rand(uint32_t lo, uint32_t hi);

#endif
