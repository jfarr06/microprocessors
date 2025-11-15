#ifndef SPRITES_H_
#define SPRITES_H_

#include <sprites/sprite.h>

#define NUM_SPRITES 1

void init_sprites();
void sprites_step();

void render_sprite(sprite* const self);

bool point_is_intersecting(sprite* const self, uint16_t px, uint16_t py);
bool is_intersecting(sprite* const self, sprite* const other);

#include <sprites/char_sprite.h>

#endif
