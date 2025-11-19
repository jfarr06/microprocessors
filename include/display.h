/**
 * @file: display.h
 * @desc: Display system interface - ST7735S LCD graphics rendering
 * @auth: James Farrelly (C24402114)
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>

#define SCREEN_W 128
#define SCREEN_H 160

#define HORIZONAL_ORIENTATION_LEFT  0
#define HORIZONAL_ORIENTATION_RIGHT 1

#define VERTICAL_ORIENTATION_DOWN 0
#define VERTICAL_ORIENTATION_UP   1

#define RGB16(r, g, b) ((g >> 5) | (g & 7) << 13 | (r >> 3) << 8 | (b >> 3) << 3)

void init_display(void);

void put_pixel(uint8_t x, uint8_t y, uint16_t colour);
void put_image(uint8_t x, uint8_t y, uint8_t width, uint8_t height,
               const uint16_t *imageBuf, uint8_t hOrientation, uint8_t vOrientation);

void draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
               uint16_t colour);

void draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t colour);
void fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t colour);

void fill_circle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t colour);

void print_text(const char *text, uint8_t len, uint8_t scale, uint8_t x, uint8_t y,
                uint16_t fgColour, uint16_t bgColour);
void print_number(uint16_t number, uint8_t scale, uint8_t x, uint8_t y,
                  uint16_t fgColour, uint16_t bgColour);

#endif
