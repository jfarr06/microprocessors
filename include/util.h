/**
 * @file: util.h
 * @desc: Utility functions - collision detection and game frame counter
 * @auth: James Farrelly (C24402114)
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * The current stepped frame counter
 * Incremented each game loop iteration for timing calculations
 */
extern uint32_t game_frame;

/**
 * Check if two rectangles intersect
 * @param x1 First rectangle X position
 * @param y1 First rectangle Y position
 * @param w1 First rectangle width
 * @param h1 First rectangle height
 * @param x2 Second rectangle X position
 * @param y2 Second rectangle Y position
 * @param w2 Second rectangle width
 * @param h2 Second rectangle height
 * @return true if rectangles overlap, false otherwise
 */
bool rect_intersects_rect(uint8_t x1, uint8_t y1, uint8_t w1, uint8_t h1,
                          uint8_t x2, uint8_t y2, uint8_t w2, uint8_t h2);

/**
 * Check if a rectangle intersects with a circle
 * @param rect_x Rectangle X position
 * @param rect_y Rectangle Y position
 * @param rect_w Rectangle width
 * @param rect_h Rectangle height
 * @param circle_x Circle center X position
 * @param circle_y Circle center Y position
 * @param circle_r Circle radius
 * @return true if rectangle and circle overlap, false otherwise
 */
bool rect_intersects_circle(uint8_t rect_x, uint8_t rect_y, 
                           uint8_t rect_w, uint8_t rect_h,
                           uint8_t circle_x, uint8_t circle_y, 
                           uint8_t circle_r);

/**
 * Simplified strlen to get a strings length.
 *
 * @param s The string
 * @return The length of the string.
 */
uint8_t strlen_custom(const char* s);

/**
 * Gets the count of digits in a 16-bit number.
 *
 * @param n The number
 * @return The count of digits in the number.
 */
uint8_t digit_count(uint16_t n);

#endif // !UTIL_H_
