#include "sprites/dg_sprite.h"
#include <io.h>
#include <images.h>
#include <sprites.h>
#include <display.h>

SPRITE_DEF(deco);
SPRITE_GET(deco);

void SPRITE_FUNC(deco, init)(sprite* const self)
{
    SET_SPRITE_IMG(self, deco1);

    self->active = true;
    self->zindex = 255; // Top of everything.
    self->horientation = self->vorientation = false;
    self->x = self->y = 50;
}

void SPRITE_FUNC(deco, step)(sprite* const self)
{
    static bool step;

    self->oldx = self->x, self->oldy = self->y;
    self->old_horientation = self->horientation, self->old_vorientation = self->vorientation;

    bool hmoved = false, vmoved = false;
    int hdirection = 0, vdirection = 0;

    if (button_right_pressed())
    {
        if (self->x < 110)
        {
            hdirection = 1;

            self->x++;
            hmoved = true;
            self->vorientation = VERTICAL_ORIENTATION_DOWN;
            self->horientation = HORIZONAL_ORIENTATION_LEFT;
        }
    }

    if (button_left_pressed())
    {
        if (self->x > 10)
        {
            self->x--;
            hmoved = true;
            self->vorientation = VERTICAL_ORIENTATION_DOWN;
            self->horientation = HORIZONAL_ORIENTATION_RIGHT;
        }
    }

    if (button_down_pressed())
    {
        if (self->y < 140)
        {
            vdirection = 1;

            self->y++;
            vmoved = true;
            self->horientation = HORIZONAL_ORIENTATION_LEFT;
            self->vorientation = VERTICAL_ORIENTATION_DOWN;
        }
    }

    if (button_up_pressed())
    {
        if (self->y > 16)
        {
            self->y--;
            vmoved = true;
            self->horientation = HORIZONAL_ORIENTATION_LEFT;
            self->vorientation = VERTICAL_ORIENTATION_UP;
        }
    }

    if (is_intersecting(self, spr_dg_get()))
    {
        if (vmoved)
        {
            if (vdirection) self->y--;
            else self->y++;

            vmoved = false;
        }

        if (hmoved)
        {
            if (hdirection) self->x--;
            else self->x++;

            hmoved = false;
        }
    }
 
    if (vmoved || hmoved) // moved at all.
    {
        if (hmoved)
        {
            if (step)
                SET_SPRITE_IMG(self, deco1);
            else
                SET_SPRITE_IMG(self, deco2);

            step ^= 1;
        }
        else {
            SET_SPRITE_IMG(self, deco3);
        }
    }

    self->render(self);       
}

sprite* SPRITE_FUNC(deco, create)()
{
    sprite* spr = NEW_SPRITE();
    spr->init = SPRITE_FUNC(deco, init);
    spr->render = default_sprite_render;
    spr->step = SPRITE_FUNC(deco, step);
    spr->has_transparency = true;

    return SPRITE(deco) = spr;
}
