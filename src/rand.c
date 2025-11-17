#include <stddef.h>

#include <rand.h>
#include <nucleo_f031k6/adc.h>
#include <nucleo_f031k6/common.h>

static uint32_t rand_seed = 0;
void init_rand()
{
    init_nucleo_f031k6_adc();

    while (!rand_seed)
        for (int i = 0; i < 10; i++)
            rand_seed += read_nucleo_f031k6_adc() << i;
}

/* Psuedo random bit shift */
uint32_t prbs()
{
    unsigned long n_bit = 0;

    n_bit = ((rand_seed & (1<<27)) >> 27) ^ ((rand_seed & (1 << 30)) >> 30);
    n_bit = ~n_bit;
    n_bit &= 1;

    rand_seed <<= 1;
    rand_seed |= n_bit;

    return rand_seed & 0x7fffffff;
}

uint32_t sys_rand(uint32_t lo, uint32_t hi)
{
    return (prbs() % (hi-lo)) + lo;
}
