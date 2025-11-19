/**
 * @file: rand.h
 * @desc: Pseudo-random number generator interface
 * @auth: James Farrelly (C24402114)
 */

#ifndef RAND_H_
#define RAND_H_

#include <stdint.h>

//=================== RANDOM

/**
 * Initializes the pseudo random number generator.
 */
void init_rand(void);

/**
 * Generates a psuedo random number between hi and lo.
 *
 * @param lo The lower bound.
 * @param hi The higher bound.
 */ 
uint32_t sys_rand(uint32_t lo, uint32_t hi);

#endif
