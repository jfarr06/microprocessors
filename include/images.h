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
#define RENDER_IMAGE(name, x, y) RENDER_IMAGE_ORIENTED(name, x, y, 0, 0)

/**
 * Render image with specified orientation
 * @param name Image name (without IMG_ prefix)
 * @param x X coordinate
 * @param y Y coordinate
 * @param h Horizontal orientation
 * @param v Vertical orientation
 */
#define RENDER_IMAGE_ORIENTED(name, x, y, h, v) (void)put_image(x, y, IMG_##name##_width, IMG_##name##_height, IMG_##name##_data, h, v)

#endif
