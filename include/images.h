#ifndef IMAGES_H_
#define IMAGES_H_

#include <images_data.h>

/**
 * Renders the specified image $name at x and y
 */
#define RENDER_IMAGE(name, x, y) RENDER_IMAGE_ORIENTED(name, x, y, 0, 0)

/**
 *
 */
#define RENDER_IMAGE_ORIENTED(name, x, y, h, v) (void)put_image(x, y, IMG_##name##_x, IMG_##name##_y, IMG_##name##_data, h, v)

#endif
