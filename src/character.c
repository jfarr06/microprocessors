/**
 * @file: character.c
 * @desc: Character system implementation - sprite animation and movement
 * @auth: James Farrelly (C24402114)
 */

#include <io.h>
#include <util.h>
#include <rooms.h>
#include <display.h>
#include <images_data.h>

#include <character.h>

#define SET_CHAR_IMG(img) do { \
    if (s_char.img_data != IMG_##img##_data) { \
            s_char.width = IMG_##img##_width; \
            s_char.height = IMG_##img##_height; \
            s_char.img_data = IMG_##img##_data; \
        } \
    } while (0)

#define CHAR_SPRITE_FRM_PROC_INT (20)
#define CHAR_SPRITE_STEP_FACT    (4)

static character s_char;

character* const get_character(void) { return &s_char; }

void render_character(void)
{
    if (s_char.oldx == s_char.x && s_char.oldy == s_char.y) goto img;

    fill_rect(s_char.oldx, s_char.oldy, s_char.width, s_char.height, COLOUR_BLACK);

img: put_image(s_char.x, s_char.y, s_char.width, s_char.height, s_char.img_data, s_char.orientation);
}

void init_character(void)
{
    SET_CHAR_IMG(char_front);

    s_char.orientation = 0;
    s_char.x = s_char.y = 50;
}

void step_character(const input_status* const input)
{
    uint8_t anim_frm = 0;
    static bool moving = false;
    static uint8_t hdirection = 0, vdirection = 0, zdirection = 0;

    if (game_frame % CHAR_SPRITE_FRM_PROC_INT == 0)
    {
        bool step = game_frame % (CHAR_SPRITE_FRM_PROC_INT * CHAR_SPRITE_STEP_FACT) == 0;

        if (step) anim_frm ^= 1;

        s_char.oldx = s_char.x, s_char.oldy = s_char.y;

        bool hmoved = false, vmoved = false;

        if (input->held & BUTTON_RIGHT)
        {
            if (s_char.x < SCREEN_W-s_char.width)
            {
                zdirection = 0;
                hdirection = 1;

                s_char.x++;
                hmoved = true;
                s_char.orientation &= ~ORIENTATION_VERTICAL;  // VERTICAL_ORIENTATION_DOWN
                s_char.orientation &= ~ORIENTATION_HORIZONTAL; // HORIZONAL_ORIENTATION_LEFT

                moving = true;
            }
        } else if (input->held & BUTTON_LEFT)
        {
            if (s_char.x > 0)
            {
                zdirection = 0;
                hdirection = 0;

                s_char.x--;
                hmoved = true;
                s_char.orientation &= ~ORIENTATION_VERTICAL;  // VERTICAL_ORIENTATION_DOWN
                s_char.orientation |= ORIENTATION_HORIZONTAL; // HORIZONAL_ORIENTATION_RIGHT

                moving = true;
            }
        } else if (input->held & BUTTON_DOWN)
        {
            if (s_char.y < SCREEN_W-s_char.height)
            {
                vdirection = 1;
                zdirection = 1;

                s_char.y++;
                vmoved = true;
                if (anim_frm) s_char.orientation |= ORIENTATION_HORIZONTAL;
                else s_char.orientation &= ~ORIENTATION_HORIZONTAL;
                s_char.orientation &= ~ORIENTATION_VERTICAL;  // VERTICAL_ORIENTATION_DOWN

                moving = true;
            }
        } else if (input->held & BUTTON_UP)
        {
            if (s_char.y > 0)
            {
                vdirection = 0;
                zdirection = 1;

                s_char.y--;
                vmoved = true;
                if (anim_frm) s_char.orientation |= ORIENTATION_HORIZONTAL;
                else s_char.orientation &= ~ORIENTATION_HORIZONTAL;
                s_char.orientation &= ~ORIENTATION_VERTICAL;  // VERTICAL_ORIENTATION_DOWN

                moving = true;
            }
        }

        if (!any_movement_input(input) && moving)
        {
            if (zdirection)
            {
                if (vdirection) SET_CHAR_IMG(char_front);
                else SET_CHAR_IMG(char_back);
            }
            else
            {
                SET_CHAR_IMG(char_right);
            }

            moving = false;
        }

        if (colliding_with_walls(&s_char))
        {
            if (vmoved)
            {
                if (vdirection) s_char.y--;
                else s_char.y++;

                vmoved = false;
            }

            if (hmoved)
            {
                if (hdirection) s_char.x--;
                else s_char.x++;

                hmoved = false;
            }
        }

        if (vmoved || hmoved) // moved at all.
        {
            if (hmoved)
            {
                if (anim_frm) SET_CHAR_IMG(char_right_mov);
                else SET_CHAR_IMG(char_right);
            }
            else {
                if (vdirection == 1)
                    SET_CHAR_IMG(char_front_mov);
                else
                    SET_CHAR_IMG(char_back_mov);
            }
        }

        render_character();
    }
}
