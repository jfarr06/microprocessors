/**
 * @file: scenes/game_scene.c
 * @desc: Game scene implementation - main gameplay scene
 * @auth: James Farrelly (C24402114)
 */

#include <stdlib.h>

#include <display.h>

#include <rooms.h>
#include <strings.h>
#include <infobox.h>
#include <character.h>

#include <scenes.h>
#include <scenes/game_scene.h>

#define NUM_OPTIONS_PAUSE (sizeof(s_pause_options)/sizeof(char*))

#define PAUSE_TITLE_OFFSET_X          CHARS(2)
#define STATUS_SCREEN_OFFSET_X        CHARS(2)
#define STATUS_SCREEN_STATUS_OFFSET_X STATUS_SCREEN_OFFSET_X+CHARS(4)
#define STATUS_SCREEN_ENTER_OFFSET_X  FONT_WIDTH*3

#define PAUSE_OPTIONS_OFFSET_Y             LINE_BIG(3)
#define STATUS_SCREEN_OFFSET_Y             LINE_BIG(4)
#define STATUS_SCREEN_ENTER_LINE1_OFFSET_Y STATUS_SCREEN_OFFSET_Y + LINE_BIG(1) + LINE(1)
#define STATUS_SCREEN_ENTER_LINE2_OFFSET_Y STATUS_SCREEN_ENTER_LINE1_OFFSET_Y + LINE(1)

#define GAME_STATE_RUNNING       0x0
#define GAME_STATE_PAUSED        0x1
#define GAME_STATE_STATUS_SCREEN 0x2

#define PAUSE_OPTION_RESUME    0x0
#define PAUSE_OPTION_RESTART   0x1
#define PAUSE_OPTION_HOME_MENU 0x2

static const char* s_pause_options[] = {
    STR_PAUSE_OPTION_RESUME,
    STR_PAUSE_OPTION_RESTART,
    STR_PAUSE_OPTION_HOME_MENU
};
static const uint8_t s_pause_option_sizes[] = {
    STR_PAUSE_OPTION_RESUME_LEN,
    STR_PAUSE_OPTION_RESTART_LEN,
    STR_PAUSE_OPTION_HOME_MENU_LEN
};
static const uint16_t s_pause_option_colours[] = { 
    COLOUR_WHITE,
    COLOUR_WHITE,
    COLOUR_WHITE
};

SCENE_GET_DEF(game);
static uint8_t s_state = GAME_STATE_RUNNING;
static uint8_t s_selected_option = 0;

static void s_reset_all_states(void)
{
    init_character();
    init_rooms();
    init_infobox(); 
}

static void s_render_game_state(void)
{
    render_character();
    render_current_room();
    render_infobox();
}

static void SCENE_F(game, s_render_pause_menu)(void)
{
    fill_rect(0, 0, SCREEN_W, SCREEN_H, COLOUR_BLACK);

    print_text(STRING(PAUSE_TITLE), 2, PAUSE_TITLE_OFFSET_X, 0, COLOUR_WHITE, COLOUR_BLACK);
    render_options(s_pause_options, s_pause_option_sizes, s_pause_option_colours, NUM_OPTIONS_PAUSE, s_selected_option, 0x0, PAUSE_OPTIONS_OFFSET_Y);
}

static void SCENE_F(game, render_status_screen)(uint8_t running_state)
{
    fill_rect(0, 0, SCREEN_W, SCREEN_H, COLOUR_BLACK);

    uint8_t offset_x = running_state == RUNNING_STATUS_WIN ? STATUS_SCREEN_OFFSET_X : STATUS_SCREEN_OFFSET_X-CHARS(1);
    print_text(STRING(STATUS_YOU), 2, offset_x, STATUS_SCREEN_OFFSET_Y, COLOUR_WHITE, COLOUR_BLACK);

    if (running_state == RUNNING_STATUS_WIN)
        print_text(STRING(STATUS_WIN), 2, STATUS_SCREEN_STATUS_OFFSET_X, STATUS_SCREEN_OFFSET_Y, COLOUR_GREEN, COLOUR_BLACK);
    else
        print_text(STRING(STATUS_LOSE), 2, STATUS_SCREEN_STATUS_OFFSET_X-CHARS(1), STATUS_SCREEN_OFFSET_Y, COLOUR_RED, COLOUR_BLACK);

    print_text(STRING(STATUS_PRESS_ENTER_LINE1), 1, STATUS_SCREEN_ENTER_OFFSET_X, STATUS_SCREEN_ENTER_LINE1_OFFSET_Y, COLOUR_WHITE, COLOUR_BLACK);
    print_text(STRING(STATUS_PRESS_ENTER_LINE2), 1, STATUS_SCREEN_ENTER_OFFSET_X, STATUS_SCREEN_ENTER_LINE2_OFFSET_Y, COLOUR_WHITE, COLOUR_BLACK);
}

void SCENE_F(game, on_change)(void) 
{
    fill_rect(0, 0, SCREEN_W, SCREEN_H, COLOUR_BLACK);

    s_reset_all_states();
    s_state = GAME_STATE_RUNNING;

    s_render_game_state();
}

static void SCENE_F(game, s_on_click_pause_menu)()
{
    switch (s_selected_option) 
    {
        case PAUSE_OPTION_RESUME:
            s_state = GAME_STATE_RUNNING;

            s_render_game_state();

            break;
        case PAUSE_OPTION_RESTART:
            s_reset_all_states();
            s_state = GAME_STATE_RUNNING;

            s_render_game_state();

            break;
        case PAUSE_OPTION_HOME_MENU:
            change_scene(SCENE(menu));

            s_reset_all_states();
            s_state = GAME_STATE_RUNNING;

            break;
    }
}

void SCENE_F(game, step)(const input_status* const input)
{
    switch (s_state) 
    {
        case GAME_STATE_RUNNING:
            if (input->trigger & BUTTON_ENTER)
            {
                s_selected_option = 0;
                s_state = GAME_STATE_PAUSED;
                SCENE_F(game, s_render_pause_menu)();

                break;
            }

            uint8_t running_status = get_running_status();
            switch (running_status) 
            {
            case RUNNING_STATUS_RUN:
                step_rooms();
                step_character(input);
                step_infobox();

                break;
            default:
                s_state = GAME_STATE_STATUS_SCREEN;
                SCENE_F(game, render_status_screen)(running_status);

                break;
            }
 
            break;
        case GAME_STATE_PAUSED:
            if (step_options(input, NUM_OPTIONS_PAUSE, &s_selected_option))
                SCENE_F(game, s_render_pause_menu)();

            if (input->trigger & BUTTON_ENTER)
                SCENE_F(game, s_on_click_pause_menu)();

            break;

        case GAME_STATE_STATUS_SCREEN:
            if (input->trigger & BUTTON_ENTER)
            {
                change_scene(SCENE(menu));

                s_reset_all_states();
                s_state = GAME_STATE_RUNNING;
            }

            break;
    }

}

void SCENE_F(game, init)(void) 
{
    s_game_scene = (scene*)malloc(sizeof(scene));
    s_game_scene->on_change = SCENE_F(game, on_change);
    s_game_scene->step = SCENE_F(game, step);
}
