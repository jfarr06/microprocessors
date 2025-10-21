#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

void init_display(void);

void put_pixel(uint16_t x, uint16_t y, uint16_t colour);
void put_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
               const uint16_t *imageBuf, int hOrientation, int vOrientation);

void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
               uint16_t colour);

void draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t colour);
void fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
               uint16_t colour);

void draw_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour);
void fill_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour);

void print_text(const char *text, const uint32_t len, uint8_t scale, uint16_t x, uint16_t y,
                uint16_t fgColour, uint16_t bgColour);
void print_centered_text(const char *text, const uint32_t len, uint8_t scale, int16_t offsetX, int16_t offsetY, uint16_t fgColour, uint16_t bgColour);
void print_number(uint16_t number, uint8_t scale, uint16_t x, uint16_t y,
                  uint16_t fgColour, uint16_t bgColour);

uint16_t rgb_to_word(uint16_t r, uint16_t g, uint16_t b);

#endif
