/**
 * @file: main.c
 * @desc: Lab 4!
 * @auth: James Farrelly (C24402114)
 */

#include <stdint.h>
#include <stm32l031xx.h>

#include <labs_common.h>
#include <nucleo_l031k6_common.h>

#define USART_TX GPIO_MODER_MODE2_Pos
#define USART_RX GPIO_MODER_MODE11_Pos

void printDecimal(uint32_t val)
{
	char str[11];

	str[10] = '\0';
	int index = 9;
	while (index >= 0)
	{
        char k = '0';

        if (val % 16 >= 10) k = 'W';

		str[index] = (val % 16) + k;
        val /= 16;

        if (val == 0) break;
		index--;
	}
    
	eputs(&str[index]);
}

int main()
{
    init_hsi16();

    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

	init_serial(GPIOA, USART_TX, GPIOA, USART_RX);
    init_common_lights();

    toggle_red(false);

	while(1)
	{
        char c = egetchar();

        if (c == 'a' || c == 'A')
        {
            toggle_red(true);
        } else {
            toggle_red(false);
        }

	}
}
