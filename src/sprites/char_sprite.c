#include <io.h>
#include <images.h>
#include <sprites.h>
#include <display.h>
#include <rooms.h>

#define CHAR_SPRITE_FRM_PROC_INT (20)
#define CHAR_SPRITE_STEP_FACT    (4)

SPRITE_DEF(char);
SPRITE_GET(char);

void SPRITE_FUNC(char, init)(sprite* const self)
{
    SET_SPRITE_IMG(self, char_front);

    self->active = true;
    self->zindex = 255; // Top of everything.
    self->horientation = self->vorientation = 0;
    self->x = self->y = 50;
}

void SPRITE_FUNC(char, step)(sprite* const self)
{
    int anim_frm = 0;
    static bool moving = false;
    static int hdirection = 0, vdirection = 0, zdirection = 0;

    if (spr_frm % CHAR_SPRITE_FRM_PROC_INT == 0)
    {
        bool step = spr_frm % (CHAR_SPRITE_FRM_PROC_INT * CHAR_SPRITE_STEP_FACT) == 0;

        if (step) anim_frm ^= 1;

        self->oldx = self->x, self->oldy = self->y;

        bool hmoved = false, vmoved = false;

        if (button_right_pressed())
        {
            if (self->x < SCREEN_W-self->width)
            {
                zdirection = 0;
                hdirection = 1;

                self->x++;
                hmoved = true;
                self->vorientation = VERTICAL_ORIENTATION_DOWN;
                self->horientation = HORIZONAL_ORIENTATION_LEFT;

                moving = true;
            }
        } else if (button_left_pressed())
        {
            if (self->x > 0)
            {
                zdirection = 0;
                hdirection = 0;

                self->x--;
                hmoved = true;
                self->vorientation = VERTICAL_ORIENTATION_DOWN;
                self->horientation = HORIZONAL_ORIENTATION_RIGHT;

                moving = true;
            }
        } else if (button_down_pressed())
        {
            if (self->y < SCREEN_W-self->height)
            {
                vdirection = 1;
                zdirection = 1;

                self->y++;
                vmoved = true;
                self->horientation = anim_frm;
                self->vorientation = VERTICAL_ORIENTATION_DOWN;

                moving = true;
            }
        } else if (button_up_pressed())
        {
            if (self->y > 0)
            {
                vdirection = 0;
                zdirection = 1;

                self->y--;
                vmoved = true;
                self->horientation = anim_frm;
                self->vorientation = VERTICAL_ORIENTATION_DOWN;

                moving = true;
            }
        }

        if (!any_movement_input() && moving)
        {
            if (zdirection)
            {
                if (vdirection) SET_SPRITE_IMG(self, char_front);
                else SET_SPRITE_IMG(self, char_back);
            }
            else
            {
                SET_SPRITE_IMG(self, char_right);
            }

            moving = false;
        }

        if (colliding_with_walls())
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
                if (anim_frm) SET_SPRITE_IMG(self, char_right_mov);
                else SET_SPRITE_IMG(self, char_right);
            }
            else {
                if (vdirection == 1)
                    SET_SPRITE_IMG(self, char_front_mov);
                else
                    SET_SPRITE_IMG(self, char_back_mov);
            }
        }

        render_sprite(self);
    }
}

sprite* SPRITE_FUNC(char, create)()
{
    sprite* spr = NEW_SPRITE();
    spr->init = SPRITE_FUNC(char, init);
    spr->step = SPRITE_FUNC(char, step);

    return SPRITE(char) = spr;
}
