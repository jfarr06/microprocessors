#include <stm32f031x6.h>
#include <nucleo_f031k6/common.h>

void pin_mode(GPIO_TypeDef* port, uint32_t bit, uint32_t mode)
{
    mode <<= bit;

    port->MODER &= ~(3u << bit);
    port->MODER |= mode;
}

void pull_mode(GPIO_TypeDef* port, uint32_t bit, uint32_t mode)
{
    mode <<= bit;

    port->PUPDR &= ~(3u << bit);
    port->PUPDR |= mode;
}

void toggle_odr_bit(GPIO_TypeDef* port, uint32_t bit, bool status)
{
    if (status)
        port->ODR |= bit;
    else
        port->ODR &= ~bit;
}

bool get_input_status(GPIO_TypeDef* port, uint32_t bit, uint8_t mode)
{
    if (mode > INPUTMODE_PULLUP) mode = INPUTMODE_PULLUP;

    return (1^mode) == (port->IDR & bit);
}
