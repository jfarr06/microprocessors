/**
 * @file: io.c
 * @desc: Input system implementation - button handling and state management
 * @auth: James Farrelly (C24402114)
 */

#include <io.h>
#include <debug.h>

#include <nucleo_f031k6/common.h>

#define STATUS(button) (get_nucleo_f031k6_idr_bit(BUTTON_##button##_PORT, BUTTON_##button##_BIT, PULLMODE_PULLUP) << BUTTON_##button##_Pos)
#define INPUT(button) do { \
        set_nucleo_f031k6_pin_mode(BUTTON_##button##_PORT, BUTTON_##button##_PINOUT, PINMODE_INPUT); \
        set_nucleo_f031k6_pull_mode(BUTTON_##button##_PORT, BUTTON_##button##_PINOUT, PULLMODE_PULLUP); \
    } while(0)

#define BUTTON_RIGHT_PORT GPIOB
#define BUTTON_LEFT_PORT  GPIOB
#define BUTTON_UP_PORT    GPIOA
#define BUTTON_DOWN_PORT  GPIOA
#define BUTTON_ENTER_PORT GPIOA

#define BUTTON_RIGHT_BIT GPIO_IDR_4
#define BUTTON_LEFT_BIT  GPIO_IDR_5
#define BUTTON_UP_BIT    GPIO_IDR_8
#define BUTTON_DOWN_BIT  GPIO_IDR_11
#define BUTTON_ENTER_BIT GPIO_IDR_0

#define BUTTON_RIGHT_PINOUT GPIO_MODER_MODER4_Pos
#define BUTTON_LEFT_PINOUT  GPIO_MODER_MODER5_Pos
#define BUTTON_UP_PINOUT    GPIO_MODER_MODER8_Pos
#define BUTTON_DOWN_PINOUT  GPIO_MODER_MODER11_Pos
#define BUTTON_ENTER_PINOUT GPIO_MODER_MODER0_Pos

static uint8_t s_current_held = 0b00000;

void init_io()
{
    DBG_INFO("Initializing inputs...");

    INPUT(ENTER);
    INPUT(RIGHT);
    INPUT(LEFT);
    INPUT(UP);
    INPUT(DOWN);
}

bool read_latest_input(input_status* status)
{
    uint8_t held = STATUS(ENTER) | 
                   STATUS(RIGHT) | 
                   STATUS(LEFT)  | 
                   STATUS(UP)    | 
                   STATUS(DOWN);

    status->held = held;
    status->trigger = ~s_current_held & held;
    status->release = s_current_held & ~held;

    s_current_held = held;

    return status != 0;
}

bool any_movement_input(const input_status* const status)
{
    return ((status->held & BUTTON_RIGHT) | 
            (status->held & BUTTON_LEFT)  | 
            (status->held & BUTTON_UP)    | 
            (status->held & BUTTON_DOWN)) != 0;
}
