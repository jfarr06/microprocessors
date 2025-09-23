/*
 * @file: main.c
 * @desc: Lab 2!
 * @auth: James Farrelly (C24402114)
 * */

#include <stdint.h>
#include <stm32l031xx.h>

#define PINMODE_INPUT (0x0)
#define PINMODE_OUTPUT (0x1)
#define PINMODE_SPECIAL (0x2)
#define PINMODE_ANALOG (0x3)

#define LIGHT_GREEN (0x0)
#define LIGHT_YELLOW (0x1)
#define LIGHT_RED (0x2)

#define BUTTON_POS (0x4)

//============= UTIL

void setLightMode(GPIO_TypeDef* port, uint32_t bit, uint8_t mode)
{
    if (mode >= 1)
        port->ODR |= (1u << bit);
    else
        port->ODR &= ~(1u << bit);
}

int getButtonStatus(GPIO_TypeDef* port, uint32_t bit)
{
    return 1^((port->IDR & (1u << bit)) >> bit);
}

void pinMode(GPIO_TypeDef* port, uint32_t bit, uint32_t mode)
{
    uint32_t current = port->MODER;

    mode <<= 2 * bit;

    current &= ~(3u << (bit * 2));
    current |= mode;

    port->MODER = current;
}

void enablePullUp(GPIO_TypeDef* port, uint32_t bit)
{
    port->PUPDR &= ~(3u << bit * 2);
    port->PUPDR |= 1U << bit * 2;
}

void delay(volatile uint32_t dly)
{
    while (--dly);
}

//================= LIGHTS

void enableRed()
{
    setLightMode(GPIOA, LIGHT_RED, 0x1);
}
void disableRed()
{
    setLightMode(GPIOA, LIGHT_RED, 0x0);
}

void enableYellow()
{
    setLightMode(GPIOA, LIGHT_YELLOW, 0x1);
}
void disableYellow()
{
    setLightMode(GPIOA, LIGHT_YELLOW, 0x0);
}

void enableGreen()
{
    setLightMode(GPIOA, LIGHT_GREEN, 0x1);
}
void disableGreen()
{
    setLightMode(GPIOA, LIGHT_GREEN, 0x0);
}

int main()
{
    static int bButtonOn;

    RCC->IOPENR |= 0b11;

    pinMode(GPIOA, 0, PINMODE_OUTPUT); // PA0 = OUTPUT
    pinMode(GPIOA, 1, PINMODE_OUTPUT); // PA1 = OUTPUT
    pinMode(GPIOA, 2, PINMODE_OUTPUT); // PA2 = OUTPUT

    pinMode(GPIOB, 3, PINMODE_OUTPUT); // PB3 = OUTPUT
    pinMode(GPIOB, 4, PINMODE_INPUT);  // PB4 = INPUT

    enablePullUp(GPIOB, BUTTON_POS);

    enableGreen();

    do
    {
        uint32_t status = getButtonStatus(GPIOB, BUTTON_POS);

        if (status)
        {
            if (!bButtonOn)
            {
                bButtonOn = 1;

                delay(200000);

                disableGreen();
                enableYellow();

                delay(200000);

                disableYellow();
                enableRed();

                delay(500000);

                enableYellow();

                delay(300000);

                disableRed();
                disableYellow();
                enableGreen();
            }
        }
        else
        {
            if (bButtonOn) bButtonOn = 0;
        }
    } while (1);
}
