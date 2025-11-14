#include <io.h>
#include <images.h>
#include <sprites.h>
#include <display.h>

#define CHAR_SPRITE_FRM_PROC_INT (15)
#define CHAR_SPRITE_STEP_FACT    (3)

SPRITE_DEF(char);
SPRITE_GET(char);

void SPRITE_FUNC(char, init)(sprite* const self)
{
    SET_SPRITE_IMG(self, char_front);

    self->active = true;
    self->zindex = 255; // Top of everything.
    self->horientation = self->vorientation = false;
    self->x = self->y = 50;
}

void SPRITE_FUNC(char, step)(sprite* const self)
{
    static bool step;

    if (spr_frm % CHAR_SPRITE_FRM_PROC_INT == 0)
    {
        self->oldx = self->x, self->oldy = self->y;

        bool hmoved = false, vmoved = false;
        int hdirection = 0, vdirection = 0;

        if (button_right_pressed())
        {
            if (self->x < SCREEN_W-self->width)
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
            if (self->x > 0)
            {
                self->x--;
                hmoved = true;
                self->vorientation = VERTICAL_ORIENTATION_DOWN;
                self->horientation = HORIZONAL_ORIENTATION_RIGHT;
            }
        }

        if (button_down_pressed())
        {
            if (self->y < SCREEN_H-self->height)
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
            if (self->y > 0)
            {
                self->y--;
                vmoved = true;
                self->horientation = HORIZONAL_ORIENTATION_LEFT;
                self->vorientation = VERTICAL_ORIENTATION_DOWN;
            }
        }

        if (vmoved || hmoved) // moved at all.
        {
            if (hmoved)
            {
                if (step && spr_frm % (CHAR_SPRITE_FRM_PROC_INT * CHAR_SPRITE_STEP_FACT) == 0)
                    SET_SPRITE_IMG(self, char_right_mov);
                else
                    SET_SPRITE_IMG(self, char_right);

            }
            else {
                if (vdirection == 1)
                    SET_SPRITE_IMG(self, char_front);
                else
                    SET_SPRITE_IMG(self, char_back);
            }

            if (spr_frm % (CHAR_SPRITE_FRM_PROC_INT * CHAR_SPRITE_STEP_FACT) == 0)
                step ^= 1;

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
