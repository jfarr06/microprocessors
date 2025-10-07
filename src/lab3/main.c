/**
 * @file: main.c
 * @desc: Lab 3!
 * @auth: James Farrelly (C24402114)
 */

#include <stdint.h>
#include <stm32l031xx.h>

#include <labs_common.h>
#include <nucleo_l031k6_common.h>

//========== DEFS

#define BUTTON1 GPIO_MODER_MODE11
#define BUTTON2 GPIO_MODER_MODE5
#define BUTTON3 GPIO_MODER_MODE4

#define BUTTON1_PINOUT GPIO_MODER_MODE11_Pos
#define BUTTON2_PINOUT GPIO_MODER_MODE5_Pos
#define BUTTON3_PINOUT GPIO_MODER_MODE4_Pos

//=============== BUTTONS

int redPressed()
{
    return get_input_status(GPIOA, BUTTON1, INPUTMODE_PULLUP);
}

int yellowPressed()
{
    return get_input_status(GPIOB, BUTTON2, INPUTMODE_PULLUP);
}

int greenPressed()
{
    return get_input_status(GPIOB, BUTTON3, INPUTMODE_PULLUP);
}

int anyPressed() { return redPressed() || yellowPressed() || greenPressed(); }

int onlyOnePressed()
{
    if (redPressed()) return !yellowPressed() && !greenPressed();
    if (yellowPressed()) return !redPressed() && !greenPressed();
    if (greenPressed()) return !redPressed() && !yellowPressed();

    return 0;
}

int currentPressed()
{
    if (!onlyOnePressed()) return -1;
    
    if (redPressed()) return RED_LIGHT_PINOUT;
    if (yellowPressed()) return YELLOW_LIGHT_PINOUT;

    return GREEN_LIGHT_PINOUT;
}

int main()
{
    const int baseTime = 1000000;
    int currentTime = baseTime;

    RCC->IOPENR |= 0b11;

    init_common_lights();

    pin_mode(GPIOA, GPIO_MODER_MODE7_Pos, PINMODE_ANALOG); // PA7 = ANALOG (RNG seed)

    pin_mode(GPIOA, BUTTON1_PINOUT, PINMODE_INPUT);  // PA11 = INPUT
    pin_mode(GPIOB, BUTTON2_PINOUT, PINMODE_INPUT);  // PB5 = INPUT
    pin_mode(GPIOB, BUTTON3_PINOUT, PINMODE_INPUT);  // PB4 = INPUT
    
    pull_mode(GPIOA, BUTTON1_PINOUT, PULLMODE_PULLUP);
    pull_mode(GPIOB, BUTTON2_PINOUT, PULLMODE_PULLUP);
    pull_mode(GPIOB, BUTTON3_PINOUT, PULLMODE_PULLUP);

    init_hsi16();
    init_adc();
    init_rand();
    init_sound(GPIOB, GPIO_MODER_MODE1_Pos);

    do
    {
        uint32_t light = rand(GREEN_LIGHT_PINOUT, RED_LIGHT_PINOUT+1);

        switch (light) 
        {
            case GREEN_LIGHT_PINOUT:
                toggle_green(true);
                break;
            case YELLOW_LIGHT_PINOUT:
                toggle_yellow(true);
                break;
            case RED_LIGHT_PINOUT:
                toggle_red(true);
                break;
        }

        int didPressCorrect = 0;
        int count = 0;

        while (++count != currentTime)
        {
            int current = currentPressed();
            if (current == -1) continue;

            if (current == light)
                didPressCorrect = 1;

            break;
        }

        if (didPressCorrect)
        {
            currentTime /= 2;

            play_sound(1000);
        }
        else 
        {
            currentTime = baseTime; // reset
            
            toggle_green(true);
            toggle_yellow(true);
            toggle_red(true);

            play_sound(400);
        }

        delay(1000000);

        play_sound(0);

        while (anyPressed()); // wait for no pressed
            
        toggle_green(false);
        toggle_yellow(false);
        toggle_red(false);

        delay(1000000);
    } while (1);
}
