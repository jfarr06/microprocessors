/**
 * @file: main.c
 * @desc: Game entrypoint
 * @auth: James Farrelly (C24402114)
 */

#include <stdint.h>
#include <stm32l031xx.h>

#include <nucleo_l031k6_common.h>

int main()
{
    init_hsi16();

    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
}
