/**
 * @file: util.c
 * @desc: Utility functions - collision detection and game frame counter
 * @auth: James Farrelly (C24402114)
 */

#include <math.h>

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

    int16_t dx, dy;
    uint16_t distance_squared;
    uint16_t radius_squared;

    uint16_t right  = (uint16_t)rect_x + (uint16_t)rect_w;
    uint16_t bottom = (uint16_t)rect_y + (uint16_t)rect_h;

    if (circle_x < rect_x) {
        closest_x = rect_x;
    } else if (circle_x > (right > 255 ? 255 : (uint8_t)right)) {
        closest_x = (right > 255 ? 255 : (uint8_t)right);
    }

    if (circle_y < rect_y) {
        closest_y = rect_y;
    } else if (circle_y > (bottom > 255 ? 255 : (uint8_t)bottom)) {
        closest_y = (bottom > 255 ? 255 : (uint8_t)bottom);
    }

    dx = (int16_t)circle_x - (int16_t)closest_x;
    dy = (int16_t)circle_y - (int16_t)closest_y;

    distance_squared = (uint16_t)(dx * dx) + (uint16_t)(dy * dy);
    radius_squared   = (uint16_t)circle_r * (uint16_t)circle_r;

    return distance_squared <= radius_squared;
}

inline uint8_t strlen_custom(const char* s)
{
    uint8_t c = 0;

    while (*(s++))
        c++;

    return c;
}

inline uint8_t digit_count(uint16_t n)
{
    if (n == 0) return 1;

    return (uint8_t)log10(n) + 1;
}
