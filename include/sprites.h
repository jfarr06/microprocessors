#ifndef SPRITES_H_
#define SPRITES_H_

#include <sprites/sprite.h>

#define NUM_SPRITES 2

void init_sprites();
void sprites_step();

void default_sprite_render(sprite* const self);
bool is_intersecting(sprite* const self, sprite* const other);

#include <sprites/dg_sprite.h>
#include <sprites/deco_sprite.h>

#endif
