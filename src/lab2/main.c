/*
 * @file: main.c
 * @desc: Lab 2!
 * @auth: James Farrelly (C24402114)
 * */

#include <stdint.h>
#include <stm32l031xx.h>

#include <labs_common.h>
#include <nucleo_l031k6_common.h>

int main()
{
    static int bButtonOn;

    enable_gpio_clocks(RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN);

    pin_mode(GPIOA, GPIO_MODER_MODE0_Pos, PINMODE_OUTPUT); // PA0 = OUTPUT
    pin_mode(GPIOA, GPIO_MODER_MODE1_Pos, PINMODE_OUTPUT); // PA1 = OUTPUT
    pin_mode(GPIOA, GPIO_MODER_MODE2_Pos, PINMODE_OUTPUT); // PA2 = OUTPUT

    pin_mode(GPIOB, GPIO_MODER_MODE3_Pos, PINMODE_OUTPUT); // PB3 = OUTPUT
    pin_mode(GPIOB, GPIO_MODER_MODE4_Pos, PINMODE_INPUT);  // PB4 = INPUT

    pull_mode(GPIOB, GPIO_PUPDR_PUPD4_Pos, PULLMODE_PULLUP);

    toggle_green(true);

    do
    {
        uint32_t status = get_input_status(GPIOB, GPIO_IDR_ID4, INPUTMODE_PULLUP);

        if (status)
        {
            if (!bButtonOn)
            {
                bButtonOn = 1;

                delay(200000);

                toggle_green(false);
                toggle_yellow(true);

                delay(200000);

                toggle_yellow(false);
                toggle_red(true);

                delay(500000);

                toggle_yellow(true);

                delay(300000);

                toggle_red(false);
                toggle_yellow(false);
                toggle_green(true);
            }
        }
        else
        {
            if (bButtonOn) bButtonOn = 0;
        }
    } while (1);
}
