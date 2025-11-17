#include <stdlib.h>

#include <display.h>
#include <font5x7.h>

#include <rooms.h>
#include <infobox.h>
#include <character.h>

#include <scenes.h>
#include <scenes/game_scene.h>

SCENE_GET_DEF(game);

void SCENE_F(game, on_change)(void) 
{
    fill_rect(0, 0, SCREEN_W, SCREEN_H, 0x0000);

    init_character();
    init_rooms();
    init_infobox();
}

void SCENE_F(game, step)(const input_status* const input)
{
    if (input->trigger & BUTTON_ENTER)
    {
        on_change_game_scene();
    }

    step_rooms();
    step_character(input);
    step_infobox();
}

void SCENE_F(game, init)(void) 
{
    s_game_scene = (scene*)malloc(sizeof(scene));
    s_game_scene->on_change = SCENE_F(game, on_change);
    s_game_scene->step = SCENE_F(game, step);
}
