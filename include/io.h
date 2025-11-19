/**
 * @file: io.h
 * @desc: Input system interface - button handling and state management
 * @auth: James Farrelly (C24402114)
 */

#ifndef IO_H_
#define IO_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * Input status structure tracking button states
 */
typedef struct input_status
{
    uint8_t held;       /* Buttons currently pressed */
    uint8_t trigger;    /* Buttons newly pressed this frame */
    uint8_t release;    /* Buttons released this frame */
} input_status;

/* Button bit positions */
#define BUTTON_ENTER_Pos (0x0)
#define BUTTON_ENTER     (0x1U << BUTTON_ENTER_Pos) /* Enter/confirm button mask */

#define BUTTON_RIGHT_Pos (0x1)
#define BUTTON_RIGHT     (0x1U << BUTTON_RIGHT_Pos) /* Right directional button mask */

#define BUTTON_LEFT_Pos (0x2)
#define BUTTON_LEFT     (0x1U << BUTTON_LEFT_Pos)   /* Left directional button mask */

#define BUTTON_UP_Pos (0x3)
#define BUTTON_UP     (0x1U << BUTTON_UP_Pos)       /* Up directional button mask */

#define BUTTON_DOWN_Pos (0x4)
#define BUTTON_DOWN     (0x1U << BUTTON_DOWN_Pos)   /* Down directional button mask */

/**
 * Initialize the input system and configure GPIO pins
 */
void init_io(void);

/**
 * Read current button states and update input status
 * @param status Pointer to input_status structure to update
 * @return true if any input detected, false otherwise
 */
bool read_latest_input(input_status* status);

/**
 * Check if any movement buttons are currently pressed
 * @param status Pointer to input_status structure
 * @return true if any movement button is held, false otherwise
 */
bool any_movement_input(const input_status* const status);

#endif
