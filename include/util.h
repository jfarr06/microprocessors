#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * The current stepped frame.
 */
extern uint32_t game_frame;

bool rect_intersects_rect(uint8_t x1, uint8_t y1, uint8_t w1, uint8_t h1,
                          uint8_t x2, uint8_t y2, uint8_t w2, uint8_t h2);

bool rect_intersects_circle(uint8_t rect_x, uint8_t rect_y, 
                           uint8_t rect_w, uint8_t rect_h,
                           uint8_t circle_x, uint8_t circle_y, 
                           uint8_t circle_r);

#endif // !UTIL_H_