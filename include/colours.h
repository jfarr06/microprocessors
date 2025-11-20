#ifndef COLOURS_H_
#define COLOURS_H_

#include <stdint.h>

/**
 * Convert 8-bit RGB values to 16-bit RGB565 color format
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 * @return 16-bit RGB565 color value
 */
#define RGB16(r, g, b) ((g >> 5) | (g & 7) << 13 | (r >> 3) << 8 | (b >> 3) << 3)

/* Common color definitions in RGB565 format using hex RGB inputs */
#define COLOUR_BLACK        ((uint16_t)RGB16(0x00, 0x00, 0x00))
#define COLOUR_WHITE        ((uint16_t)RGB16(0xFF, 0xFF, 0xFF))
#define COLOUR_RED          ((uint16_t)RGB16(0xFF, 0x00, 0x00))
#define COLOUR_LIME         ((uint16_t)RGB16(0x00, 0xFF, 0x00))
#define COLOUR_BLUE         ((uint16_t)RGB16(0x00, 0x00, 0xFF))
#define COLOUR_YELLOW       ((uint16_t)RGB16(0xFF, 0xFF, 0x00))
#define COLOUR_CYAN         ((uint16_t)RGB16(0x00, 0xFF, 0xFF))
#define COLOUR_MAGENTA      ((uint16_t)RGB16(0xFF, 0x00, 0xFF))
#define COLOUR_SILVER       ((uint16_t)RGB16(0xC0, 0xC0, 0xC0))
#define COLOUR_GRAY         ((uint16_t)RGB16(0x80, 0x80, 0x80))
#define COLOUR_MAROON       ((uint16_t)RGB16(0x80, 0x00, 0x00))
#define COLOUR_OLIVE        ((uint16_t)RGB16(0x80, 0x80, 0x00))
#define COLOUR_GREEN        ((uint16_t)RGB16(0x00, 0x80, 0x00))
#define COLOUR_PURPLE       ((uint16_t)RGB16(0x80, 0x00, 0x80))
#define COLOUR_TEAL         ((uint16_t)RGB16(0x00, 0x80, 0x80))
#define COLOUR_NAVY         ((uint16_t)RGB16(0x00, 0x00, 0x80))

/* Additional common UI colors */
#define COLOUR_ORANGE       ((uint16_t)RGB16(0xFF, 0xA5, 0x00))
#define COLOUR_PINK         ((uint16_t)RGB16(0xFF, 0xC0, 0xCB))
#define COLOUR_BROWN        ((uint16_t)RGB16(0xA5, 0x2A, 0x2A))
#define COLOUR_GOLD         ((uint16_t)RGB16(0xFF, 0xD7, 0x00))
#define COLOUR_SKY_BLUE     ((uint16_t)RGB16(0x87, 0xCE, 0xEB))
#define COLOUR_VIOLET       ((uint16_t)RGB16(0xEE, 0x82, 0xEE))

#endif // !COLOURS_H_
