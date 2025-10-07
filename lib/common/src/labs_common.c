#include <labs_common.h>
#include <nucleo_l031k6_common.h>

void delay(volatile uint32_t dly)
{
    while (--dly);
}

void init_common_lights()
{
    pin_mode(GPIOA, GREEN_LIGHT_PINOUT, PINMODE_OUTPUT);    // PA0 = OUTPUT
    pin_mode(GPIOA, YELLOW_LIGHT_PINOUT, PINMODE_OUTPUT);   // PA1 = OUTPUT
    pin_mode(GPIOA, RED_LIGHT_PINOUT, PINMODE_OUTPUT);      // PA2 = OUTPUT
}

void toggle_green(bool status)
{
    toggle_odr_bit(GPIOA, GREEN_LIGHT, status);
}

void toggle_yellow(bool status)
{
    toggle_odr_bit(GPIOA, YELLOW_LIGHT, status);
}

void toggle_red(bool status)
{
    toggle_odr_bit(GPIOA, RED_LIGHT, status);
}

void eputchar(char c)
{
    while (!(USART2->ISR & USART_ISR_TC)); // Wait for transmit to finish.

    USART2->TDR = c;
}

char egetchar()
{
    while (!(USART2->ISR & USART_ISR_RXNE)); // Wait for input.

    return USART2->RDR;
}

void eputs(char *str)
{
	while(*str) // keep printing until a NULL is found
	{
		eputchar(*str);

		str++;
	}
}

static uint32_t rand_seed = 0;
void init_rand()
{
    while (!rand_seed)
        for (int i = 0; i < 10; i++)
            rand_seed += read_adc() << i;
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

uint32_t rand(uint32_t lo, uint32_t hi)
{
    return (prbs() % (hi-lo)) + lo;
}
