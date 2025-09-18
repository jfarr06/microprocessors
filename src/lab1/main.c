/*
 * @file: main.c
 * @desc: Main introduction for microprocessors module
 * @auth: James Farrelly (C24402114)
 * */

#include <stm32l031xx.h>

void delay(unsigned int ms)
{
    while (ms--);
}

int main()
{
    RCC->IOPENR |= (1 << 1);
    GPIOB->MODER = (1 << 6);
    
    while (1)
    {
        GPIOB->ODR = (1 << 3);

        delay(1000000);

        GPIOB->ODR = 0x0;

        delay(1000000);
    }
}
