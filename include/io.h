/**
 * @file: io.h
 * @desc: Input system interface - button handling and state management
 * @auth: James Farrelly (C24402114)
 */

#ifndef IO_H_
#define IO_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct input_status
{
    uint8_t held;
    uint8_t trigger;
    uint8_t release;
} input_status;

#define BUTTON_ENTER_Pos (0x0)
#define BUTTON_ENTER     (0x1U << BUTTON_ENTER_Pos)

#define BUTTON_RIGHT_Pos (0x1)
#define BUTTON_RIGHT     (0x1U << BUTTON_RIGHT_Pos)

#define BUTTON_LEFT_Pos (0x2)
#define BUTTON_LEFT     (0x1U << BUTTON_LEFT_Pos)

#define BUTTON_UP_Pos (0x3)
#define BUTTON_UP     (0x1U << BUTTON_UP_Pos)

#define BUTTON_DOWN_Pos (0x4)
#define BUTTON_DOWN     (0x1U << BUTTON_DOWN_Pos)

void init_io(void);
bool read_latest_input(input_status* status);

bool any_movement_input(const input_status* const status);

#endif
