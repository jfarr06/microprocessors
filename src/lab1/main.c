/*
 * @file: main.c
 * @desc: Main introduction for microprocessors module
 * @auth: James Farrelly (C24402114)
 * */

#include <stm32l031xx.h>

void delay(volatile unsigned int ms)
{
    while (ms--);
}

int main()
{
    RCC->IOPENR |= (1 << 1); // Enable clock
    GPIOB->MODER = (1 << 6); // Set mode
    
    while (1)
    {
        GPIOB->ODR = (1 << 3); // Turn on LED

        delay(1000000);

        GPIOB->ODR = 0x0; // Turn off LED

        delay(1000000);
    }
}
