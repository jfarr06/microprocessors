#ifndef SPRITES_SPRITE_H_
#define SPRITES_SPRITE_H_

#include <stdint.h>
#include <stdbool.h>

#define SPRITE(x) spr_##x
#define SPRITE_DEF(x) static sprite* SPRITE(x)
#define SPRITE_FUNC(x, func) spr_##x##_##func
#define SPRITE_GET(x) sprite* SPRITE_FUNC(x, get)() { return SPRITE(x); }
#define SET_SPRITE_IMG(sprite, img) do { \
    if ((sprite)->img_data != IMG_##img##_data) { \
            (sprite)->width = IMG_##img##_width; \
            (sprite)->height = IMG_##img##_height; \
            (sprite)->img_data = IMG_##img##_data; \
        } \
    } while (0)

#define NEW_SPRITE() (sprite*)malloc(sizeof(sprite))

typedef struct sprite sprite;

/* sprite step does not auto render, you must do that yourself. SORRY! */

struct sprite {
    uint16_t width, height, old_width, old_height;
    const uint16_t* img_data;
    uint16_t x, y;
    int oldx, oldy;
    uint16_t horientation, vorientation;
    uint8_t zindex;
    bool active;

    void (*init)(sprite* const self);
    void (*step)(sprite* const self);
};

extern uint32_t spr_frm;

#endif
