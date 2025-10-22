#include <io.h>
#include <debug.h>

#include <nucleo_f031k6/common.h>

#define BUTTON_RIGHT GPIO_IDR_4
#define BUTTON_LEFT  GPIO_IDR_5
#define BUTTON_UP    GPIO_IDR_8
#define BUTTON_DOWN  GPIO_IDR_11
#define BUTTON_ENTER GPIO_IDR_0

#define BUTTON_RIGHT_PINOUT GPIO_MODER_MODER4_Pos
#define BUTTON_LEFT_PINOUT  GPIO_MODER_MODER5_Pos
#define BUTTON_UP_PINOUT    GPIO_MODER_MODER8_Pos
#define BUTTON_DOWN_PINOUT  GPIO_MODER_MODER11_Pos
#define BUTTON_ENTER_PINOUT GPIO_MODER_MODER0_Pos

void init_io()
{
    DBG_INFO("Initializing inputs...");

    pin_mode(GPIOB, BUTTON_RIGHT_PINOUT, PINMODE_INPUT);
    pin_mode(GPIOB, BUTTON_LEFT_PINOUT, PINMODE_INPUT);
    pin_mode(GPIOA, BUTTON_UP_PINOUT, PINMODE_INPUT);
    pin_mode(GPIOA, BUTTON_DOWN_PINOUT, PINMODE_INPUT);
    pin_mode(GPIOA, BUTTON_ENTER_PINOUT, PINMODE_INPUT);

    pull_mode(GPIOB, BUTTON_RIGHT_PINOUT, PULLMODE_PULLUP);
    pull_mode(GPIOB, BUTTON_LEFT_PINOUT, PULLMODE_PULLUP);
    pull_mode(GPIOA, BUTTON_UP_PINOUT, PULLMODE_PULLUP);
    pull_mode(GPIOA, BUTTON_DOWN_PINOUT, PULLMODE_PULLUP);
    pull_mode(GPIOA, BUTTON_ENTER_PINOUT, PULLMODE_PULLUP);
}

bool button_right_pressed()
{
    return get_input_status(GPIOB, BUTTON_RIGHT, PULLMODE_PULLUP);
}

bool button_left_pressed()
{
    return get_input_status(GPIOB, BUTTON_LEFT, PULLMODE_PULLUP);
}

bool button_up_pressed()
{
    return get_input_status(GPIOA, BUTTON_UP, PULLMODE_PULLUP);
}

bool button_down_pressed()
{
    return get_input_status(GPIOA, BUTTON_DOWN, PULLMODE_PULLUP);
}

bool button_enter_pressed()
{
    return get_input_status(GPIOA, BUTTON_ENTER, PULLMODE_PULLUP);
}

bool any_input()
{
    return button_left_pressed() || button_right_pressed() || button_up_pressed() || button_down_pressed() || button_enter_pressed();
}
