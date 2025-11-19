/**
 * @file: images.h
 * @desc: Image rendering macros for sprite display
 * @auth: James Farrelly (C24402114)
 */

#ifndef IMAGES_H_
#define IMAGES_H_

#include <images_data.h>

/**
 * Render image at specified coordinates with default orientation
 * @param name Image name (without IMG_ prefix)
 * @param x X coordinate
 * @param y Y coordinate
 */
#define RENDER_IMAGE(name, x, y) RENDER_IMAGE_ORIENTED(name, x, y, 0)

/**
 * Render image with specified orientation
 * @param name Image name (without IMG_ prefix)
 * @param x X coordinate
 * @param y Y coordinate
 * @param orientation Orientation flags (bit 0=horizontal, bit 1=vertical)
 */
#define RENDER_IMAGE_ORIENTED(name, x, y, orientation) (void)put_image(x, y, IMG_##name##_width, IMG_##name##_height, IMG_##name##_data, orientation)

#endif
