/**
 * @file: util.c
 * @desc: Utility functions - collision detection and game frame counter
 * @auth: James Farrelly (C24402114)
 */

#include <util.h>

uint32_t game_frame = 0;

inline bool rect_intersects_rect(uint8_t x1, uint8_t y1, uint8_t w1, uint8_t h1,
                          uint8_t x2, uint8_t y2, uint8_t w2, uint8_t h2)
{
    return !(x1 + w1 < x2 || x2 + w2 < x1 || y1 + h1 < y2 || y2 + h2 < y1);
}

inline bool rect_intersects_circle(uint8_t rect_x, uint8_t rect_y, 
                           uint8_t rect_w, uint8_t rect_h,
                           uint8_t circle_x, uint8_t circle_y, 
                           uint8_t circle_r)
{
    uint8_t closest_x = circle_x;
    uint8_t closest_y = circle_y;
    
    if (circle_x < rect_x) {
        closest_x = rect_x;
    } else if (circle_x > rect_x + rect_w) {
        closest_x = rect_x + rect_w;
    }
    
    if (circle_y < rect_y) {
        closest_y = rect_y;
    } else if (circle_y > rect_y + rect_h) {
        closest_y = rect_y + rect_h;
    }
    
    uint8_t dx = circle_x - closest_x;
    uint8_t dy = circle_y - closest_y;
    uint16_t distance_squared = dx * dx + dy * dy;
    uint16_t radius_squared = circle_r * circle_r;
    
    return distance_squared <= radius_squared;
}