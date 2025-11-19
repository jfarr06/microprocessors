/**
 * @file: display.h
 * @desc: Display system interface - ST7735S LCD graphics rendering
 * @auth: James Farrelly (C24402114)
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>

/* Screen dimensions */
#define SCREEN_W 128
#define SCREEN_H 160

/* Legacy orientation flags (deprecated - use ORIENTATION_* bit masks instead) */
#define HORIZONAL_ORIENTATION_LEFT  0
#define HORIZONAL_ORIENTATION_RIGHT 1
#define VERTICAL_ORIENTATION_DOWN 0
#define VERTICAL_ORIENTATION_UP   1

/* Orientation bit masks for sprite rendering */
#define ORIENTATION_HORIZONTAL (0x1U << 0) /* Bit 0: horizontal orientation (0=left, 1=right) */
#define ORIENTATION_VERTICAL   (0x1U << 1) /* Bit 1: vertical orientation (0=down, 1=up) */

/**
 * Convert 8-bit RGB values to 16-bit RGB565 color format
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 * @return 16-bit RGB565 color value
 */
#define RGB16(r, g, b) ((g >> 5) | (g & 7) << 13 | (r >> 3) << 8 | (b >> 3) << 3)

/**
 * Initialize the ST7735S display controller
 */
void init_display(void);

/**
 * Draw a single pixel at the specified coordinates
 * @param x X coordinate
 * @param y Y coordinate
 * @param colour 16-bit RGB565 color value
 */
void put_pixel(uint8_t x, uint8_t y, uint16_t colour);

/**
 * Draw an image sprite at the specified position with optional orientation
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param width Image width in pixels
 * @param height Image height in pixels
 * @param imageBuf Pointer to image data buffer
 * @param orientation Orientation flags (bit 0=horizontal, bit 1=vertical)
 */
void put_image(uint8_t x, uint8_t y, uint8_t width, uint8_t height,
               const uint16_t *imageBuf, uint8_t orientation);

/**
 * Draw a line between two points
 * @param x0 Starting X coordinate
 * @param y0 Starting Y coordinate
 * @param x1 Ending X coordinate
 * @param y1 Ending Y coordinate
 * @param colour 16-bit RGB565 color value
 */
void draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
               uint16_t colour);

/**
 * Draw a rectangle outline
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Width in pixels
 * @param h Height in pixels
 * @param colour 16-bit RGB565 color value
 */
void draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t colour);

/**
 * Draw a filled rectangle
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Width in pixels
 * @param h Height in pixels
 * @param colour 16-bit RGB565 color value
 */
void fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t colour);

/**
 * Draw a filled circle
 * @param x0 Center X coordinate
 * @param y0 Center Y coordinate
 * @param radius Circle radius in pixels
 * @param colour 16-bit RGB565 color value
 */
void fill_circle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t colour);

/**
 * Print text string to display using bitmap font
 * @param text String to print
 * @param len Length of string
 * @param scale Font scale factor (1=5x7, 2=10x14, etc.)
 * @param x X coordinate
 * @param y Y coordinate
 * @param fgColour Foreground color (16-bit RGB565)
 * @param bgColour Background color (16-bit RGB565)
 */
void print_text(const char *text, uint8_t len, uint8_t scale, uint8_t x, uint8_t y,
                uint16_t fgColour, uint16_t bgColour);

/**
 * Print numeric value to display using bitmap font
 * @param number Number to print
 * @param scale Font scale factor
 * @param x X coordinate
 * @param y Y coordinate
 * @param fgColour Foreground color (16-bit RGB565)
 * @param bgColour Background color (16-bit RGB565)
 */
void print_number(uint16_t number, uint8_t scale, uint8_t x, uint8_t y,
                  uint16_t fgColour, uint16_t bgColour);

#endif
