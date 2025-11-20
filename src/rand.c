/**
 * @file: rand.c
 * @desc: Pseudo-random number generator implementation
 * @auth: James Farrelly (C24402114)
 */

#include <stddef.h>

#include <rand.h>
#include <debug.h>
#include <nucleo_f031k6/adc.h>
#include <nucleo_f031k6/common.h>

static uint32_t s_rand_seed = 0;
void init_rand()
{
    DBG_INFO("Initializing random number generator...");
    init_nucleo_f031k6_adc();

    while (!s_rand_seed)
        for (int i = 0; i < 10; i++)
            s_rand_seed += read_nucleo_f031k6_adc() << i;
    
    DBG_INFO("Random seed initialized: %p", s_rand_seed);
}

/* Psuedo random bit shift */
uint32_t prbs()
{
    unsigned long n_bit = 0;

    n_bit = ((s_rand_seed & (1<<27)) >> 27) ^ ((s_rand_seed & (1 << 30)) >> 30);
    n_bit = ~n_bit;
    n_bit &= 1;

    s_rand_seed <<= 1;
    s_rand_seed |= n_bit;

    return s_rand_seed & 0x7fffffff;
}

uint32_t sys_rand(uint32_t lo, uint32_t hi)
{
    return (prbs() % (hi-lo)) + lo;
}

bool rand_chance(uint8_t percent)
{
    if (percent == 0)
        return false;
    if (percent >= 100)
        return true;

    /* Generate a number in [0, 99] and compare to percent */
    uint32_t r = sys_rand(0, 100);
    return r < percent; 
}
