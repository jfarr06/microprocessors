/**
 * @file: scenes/menu_scene.c
 * @desc: Menu scene implementation - home menu and mode selection
 * @auth: James Farrelly (C24402114)
 */

#include "colours.h"
#include "rand.h"
#include "rooms.h"
#include <stdlib.h>

#include <display.h>

#include <scenes.h>
#include <strings.h>
#include <infobox.h>
#include <scenes/menu_scene.h>
#include <music.h>
#include <music_tracks.h>

#define NUM_OPTIONS_MODE_SELECT (sizeof(s_mode_select_options)/sizeof(char*))

#define MENU_STATE_HOME_MENU   0x0
#define MENU_STATE_MODE_SELECT 0x1

#define TITLE_SCALE       2
#define PLAY_SCALE        2
#define MODE_SELECT_SCALE 2
#define MODE_DESC_SCALE   1

#define TITLE_OFFSET_X CHARS(1)+10
#define PLAY_OFFSET_X  CHARS(2)-(FONT_WIDTH/2)+4

#define TITLE_TOP_OFFSET_Y           4
#define TITLE_BOTTOM_OFFSET_Y        LINE_BIG(1) + TITLE_TOP_OFFSET_Y*2
#define PLAY_OFFSET_Y                LINE_BIG(5)
#define MODE_SELECT_OPTIONS_OFFSET_Y LINE_BIG(2)
#define MODE_DESC_OFFSET_Y(x)        LINE_BIG(NUM_OPTIONS_MODE_SELECT+2) + LINE(1) + NUM_OPTIONS_MODE_SELECT+2 + (LINE(1) + 2)*x

#define TITLE_INNER_W SCREEN_W-1
#define TITLE_INNER_H LINE_BIG(2)+(TITLE_TOP_OFFSET_Y*2)+2

#define TITLE_INNER_DIMENSIONS TITLE_INNER_W, TITLE_INNER_H

#define MODE_SELECT_OPTION_EASY    0x0
#define MODE_SELECT_OPTION_MEDIUM  0x1
#define MODE_SELECT_OPTION_HARD    0x2
#define MODE_SELECT_OPTION_ENDLESS 0x3
#define MODE_SELECT_OPTION_RANDOM  0x4
#define MODE_SELECT_OPTION_RETURN  0x5

static const char* s_mode_select_options[] = {
    STR_MODE_SELECT_EASY, 
    STR_MODE_SELECT_MEDIUM,
    STR_MODE_SELECT_HARD, 
    STR_MODE_SELECT_ENDLESS,
    STR_MODE_SELECT_RANDOM,
    STR_MODE_SELECT_RETURN
};
static const uint8_t s_mode_select_option_sizes[] = {
    STR_MODE_SELECT_EASY_LEN, 
    STR_MODE_SELECT_MEDIUM_LEN,
    STR_MODE_SELECT_HARD_LEN, 
    STR_MODE_SELECT_ENDLESS_LEN, 
    STR_MODE_SELECT_RANDOM_LEN,
    STR_MODE_SELECT_RETURN_LEN
};
static const uint16_t s_mode_select_option_colours[] = { 
    COLOUR_LIME, 
    COLOUR_YELLOW, 
    COLOUR_RED, 
    COLOUR_ORANGE,
    COLOUR_CYAN, 
    COLOUR_WHITE
};

static const uint8_t s_mode_values[3][3] =
{
    // coins%, time, num coins
    {50, 120, 15}, // Easy
    {20, 120, 15}, // Medium
    {15, 60, 15}   // Hard
};

SCENE_GET_DEF(menu);
static uint8_t s_state = MENU_STATE_HOME_MENU;
static uint8_t s_selected_option = 0;

static void SCENE_F(menu, s_render)()
{
    fill_rect(0, 0, SCREEN_W, SCREEN_H, 0x0000);

    switch (s_state)
    {
    case MENU_STATE_HOME_MENU:
        draw_rect(0, 0, TITLE_INNER_DIMENSIONS, COLOUR_WHITE);

        print_text(STRING(MENU_TITLE_TOP), TITLE_SCALE, TITLE_OFFSET_X, TITLE_TOP_OFFSET_Y, COLOUR_RED, COLOUR_BLACK);
        print_text(STRING(MENU_TITLE_BOTTOM), TITLE_SCALE, TITLE_OFFSET_X, TITLE_BOTTOM_OFFSET_Y, COLOUR_WHITE, COLOUR_BLACK);

        print_text(STRING(MENU_PLAY), PLAY_SCALE, PLAY_OFFSET_X, PLAY_OFFSET_Y, COLOUR_WHITE, COLOUR_BLACK);

        break;
    case MENU_STATE_MODE_SELECT:
        print_text(STRING(MODE_SELECT_TITLE), MODE_SELECT_SCALE, 0, 0, COLOUR_WHITE, COLOUR_BLACK);
        render_options(s_mode_select_options, s_mode_select_option_sizes, s_mode_select_option_colours, NUM_OPTIONS_MODE_SELECT, s_selected_option, 0x0, MODE_SELECT_OPTIONS_OFFSET_Y);

        switch (s_selected_option)
        {
        case MODE_SELECT_OPTION_EASY: // Easy
            print_text(STRING(DESC_LOTS_OF_COINS), MODE_DESC_SCALE, 0, MODE_DESC_OFFSET_Y(0), COLOUR_WHITE, COLOUR_BLACK);
            print_text(STRING(DESC_LOTS_OF_TIME), MODE_DESC_SCALE, 0, MODE_DESC_OFFSET_Y(1), COLOUR_WHITE, COLOUR_BLACK);

            break;
        case MODE_SELECT_OPTION_MEDIUM: // Medium
            print_text(STRING(DESC_REDUCED_COINS), MODE_DESC_SCALE, 0, MODE_DESC_OFFSET_Y(0), COLOUR_WHITE, COLOUR_BLACK);
            print_text(STRING(DESC_LOTS_OF_TIME), MODE_DESC_SCALE, 0, MODE_DESC_OFFSET_Y(1), COLOUR_WHITE, COLOUR_BLACK);

            break;
        case MODE_SELECT_OPTION_HARD: // Hard
            print_text(STRING(DESC_REDUCED_COINS), MODE_DESC_SCALE, 0, MODE_DESC_OFFSET_Y(0), COLOUR_WHITE, COLOUR_BLACK);
            print_text(STRING(DESC_REDUCED_TIME), MODE_DESC_SCALE, 0, MODE_DESC_OFFSET_Y(1), COLOUR_WHITE, COLOUR_BLACK);

            break;
        case MODE_SELECT_OPTION_ENDLESS: // Endless
            print_text(STRING(DESC_REDUCED_COINS), MODE_DESC_SCALE, 0, MODE_DESC_OFFSET_Y(0), COLOUR_WHITE, COLOUR_BLACK);
            print_text(STRING(DESC_NO_TIME_LIMIT), MODE_DESC_SCALE, 0, MODE_DESC_OFFSET_Y(1), COLOUR_WHITE, COLOUR_BLACK);

            break;
        case MODE_SELECT_OPTION_RANDOM: // Random
            print_text(STRING(DESC_RANDOM_COINS), MODE_DESC_SCALE, 0, MODE_DESC_OFFSET_Y(0), COLOUR_WHITE, COLOUR_BLACK);
            print_text(STRING(DESC_RANDOM_TIME_LIMIT), MODE_DESC_SCALE, 0, MODE_DESC_OFFSET_Y(1), COLOUR_WHITE, COLOUR_BLACK);
            
            break;
        case MODE_SELECT_OPTION_RETURN: // Return
            print_text(STRING(DESC_RETURN_TO_MENU), MODE_DESC_SCALE, 0, MODE_DESC_OFFSET_Y(0), COLOUR_WHITE, COLOUR_BLACK);

            break;
        }

        break;
    default:
        break;
    }
}

void SCENE_F(menu, on_change)()
{
    s_selected_option = 0;

    /* Start menu background music */
    play_music(get_menu_music());

    SCENE_F(menu, s_render)();
}

static void home_menu_on_click()
{
    s_state = MENU_STATE_MODE_SELECT;
    s_selected_option = 0;

    SCENE_F(menu, s_render)();
}

static void mode_select_on_click()
{
    switch (s_selected_option)
    {
    case MODE_SELECT_OPTION_ENDLESS:
        s_state = MENU_STATE_HOME_MENU;

        set_coin_generation_chance(25);
        set_infobox_start_time(0);
        set_infobox_coins(0);

        change_scene(SCENE(game));

        break;
    case MODE_SELECT_OPTION_RANDOM:
        s_state = MENU_STATE_HOME_MENU;

        set_coin_generation_chance(sys_rand(5, 41));
        set_infobox_start_time(sys_rand(15, 60*5));
        set_infobox_coins(sys_rand(1, 51));

        change_scene(SCENE(game));

        break;
    case MODE_SELECT_OPTION_RETURN: // Return
        s_state = MENU_STATE_HOME_MENU;

        s_selected_option = 0;
        SCENE_F(menu, s_render)();

        break;
    default:
        s_state = MENU_STATE_HOME_MENU;

        const uint8_t* mode_values = s_mode_values[s_selected_option];

        set_coin_generation_chance(mode_values[0]);
        set_infobox_start_time(mode_values[1]);
        set_infobox_coins(mode_values[2]);

        change_scene(SCENE(game));

        break;
    }
}

void SCENE_F(menu, step)(const input_status* const input)
{
    switch (s_state)
    {
    case MENU_STATE_HOME_MENU:
        if (input->trigger & BUTTON_ENTER)
            home_menu_on_click();

        break;
    case MENU_STATE_MODE_SELECT:
        if (step_options(input, NUM_OPTIONS_MODE_SELECT, &s_selected_option))
            s_render_menu_scene();

        if (input->trigger & BUTTON_ENTER)
            mode_select_on_click();

        break;
    default:
        break;
    }
}

void SCENE_F(menu, init)(void) 
{
    s_menu_scene = (scene*)malloc(sizeof(scene));
    s_menu_scene->on_change = SCENE_F(menu, on_change);
    s_menu_scene->step = SCENE_F(menu, step);
}
