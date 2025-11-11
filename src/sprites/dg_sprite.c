#include <io.h>
#include <images.h>
#include <sprites.h>

SPRITE_DEF(dg);
SPRITE_GET(dg);

void SPRITE_FUNC(dg, init)(sprite* const self)
{
    SET_SPRITE_IMG(self, dg1);

    self->active = true;
    self->zindex = 0; // Bottom of everything.
    self->horientation = self->vorientation = 0;
    self->x = 20, self->y = 80;
    self->oldx = -1, self->oldy = -1;
}

void SPRITE_FUNC(dg, step)(sprite* const self)
{
    self->render(self);       
}

sprite* SPRITE_FUNC(dg, create)()
{
    sprite* spr = NEW_SPRITE();
    spr->init = SPRITE_FUNC(dg, init);
    spr->render = default_sprite_render;
    spr->step = SPRITE_FUNC(dg, step);
    spr->has_transparency = false;

    return SPRITE(dg) = spr;
}
