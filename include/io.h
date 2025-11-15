#ifndef IO_H_
#define IO_H_

#include <stdbool.h>

void init_io();

bool button_right_pressed();
bool button_left_pressed();
bool button_up_pressed();
bool button_down_pressed();
bool button_enter_pressed();
bool any_movement_input();
bool any_input();

#endif
