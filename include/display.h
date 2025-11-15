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

void init_display(void);

void put_pixel(uint16_t x, uint16_t y, uint16_t colour);
void put_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
               const uint16_t *imageBuf, int hOrientation, int vOrientation);

void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
               uint16_t colour);

void draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t colour);
void fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
               uint16_t colour);

void fill_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour);

void print_text(const char *text, uint32_t len, uint8_t scale, uint16_t x, uint16_t y,
                uint16_t fgColour, uint16_t bgColour);
void print_number(uint16_t number, uint8_t scale, uint16_t x, uint16_t y,
                  uint16_t fgColour, uint16_t bgColour);

uint16_t rgb_to_word(uint16_t r, uint16_t g, uint16_t b);

#endif
