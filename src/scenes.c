/**
 * @file: scenes.c
 * @desc: Scene management system implementation
 * @auth: James Farrelly (C24402114)
 */

#include <scenes.h>
#include <strings.h>
#include <display.h>
#include <music.h>
#include <notes.h>

SCENE_GET_DEF(current); // psuedo scene

void init_scenes()
{
    SCENE_F(menu, init)();
    SCENE_F(game, init)();
}

void step_scenes(const input_status* const input)
{
    if (!s_current_scene) return;

    s_current_scene->step(input);
}

void change_scene(scene* scene)
{
    if (s_current_scene != scene)
    {
        s_current_scene = scene;
        s_current_scene->on_change();
    }
}

bool step_options(const input_status* const input, uint8_t num_options, uint8_t* selected_option)
{
    if (input->trigger & BUTTON_UP)
    {
        if (*selected_option == 0)
            *selected_option = num_options - 1;
        else
            --*selected_option;

        /* Play navigation sound effect */
        play_sound_effect(C5, 50);

        return true;
    } else if (input->trigger & BUTTON_DOWN)
    {
        if (*selected_option == num_options - 1)
            *selected_option = 0;
        else
            ++*selected_option;

        /* Play navigation sound effect */
        play_sound_effect(C5, 50);

        return true;
    }

    return false;
}

void render_options(
    const char* const options[], 
    const uint8_t option_sizes[],
    const uint16_t option_colours[],
    const uint8_t num_options, 
    const uint8_t selected_option, 
    const uint8_t xOffset, 
    const uint8_t yOffset
) {
   for (uint8_t i = 0; i < num_options; i++)
    {
        if (i == selected_option)
            print_text(STRING(SELECT_CURRENT_OPTION), 2, xOffset, LINE_BIG(i) + yOffset + i*2, COLOUR_WHITE, COLOUR_BLACK);

        print_text(options[i], option_sizes[i], 2, CHARS(1) + xOffset, LINE_BIG(i) + yOffset + i*2, option_colours[i], COLOUR_BLACK);
    } 
}
