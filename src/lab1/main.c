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
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN; // Enable clock
    GPIOB->MODER = GPIO_MODER_MODE3_1; // Set mode
    
    while (1)
    {
        GPIOB->ODR = GPIO_ODR_OD3; // Turn on LED

        delay(1000000);

        GPIOB->ODR = 0x0; // Turn off LED

        delay(1000000);
    }
}
