/**
 * @file: scenes/menu_scene.c
 * @desc: Menu scene implementation - home menu and mode selection
 * @auth: James Farrelly (C24402114)
 */

#include <stdlib.h>

#include <display.h>
#include <font5x7.h>

#include <scenes.h>
#include <infobox.h>
#include <scenes/menu_scene.h>

SCENE_GET_DEF(menu);

static const char* s_home_menu_options[] = {"Play", "About"};
static const int s_home_menu_option_sizes[] = {4, 5};
static const uint16_t s_home_menu_option_colours[] = {0xffff, 0xffff};

static const char* s_mode_select_options[] = {"Easy", "Medium", "Hard", "Endless", "Random", "Return"};
static const int s_mode_select_option_sizes[] = {4, 6, 4, 7, 6, 6};
static const uint16_t s_mode_select_option_colours[] = { 
    RGB16(0x00, 0xff, 0x00), 
    RGB16(0xff, 0xff, 0x00), 
    RGB16(0xff, 0x00, 0x00), 
    RGB16(0xfc, 0x7b, 0x03),
    RGB16(0x03, 0xdb, 0xfc), 
    0xffff 
};

#define NUM_OPTIONS_HOME_MENU   (sizeof(s_home_menu_options)/sizeof(char*))
#define NUM_OPTIONS_MODE_SELECT (sizeof(s_mode_select_options)/sizeof(char*))

#define STATUS_HOME_MENU   0x0
#define STATUS_MODE_SELECT 0x1
#define STATUS_ABOUT       0x2

static uint8_t s_status = STATUS_HOME_MENU;
static uint8_t s_selected_option = 0;

#define LINE(x) ((x) * FONT_HEIGHT)

#define LINE_BIG(x) (LINE(x) * 2)
#define CHARS(x) ((x) * FONT_WIDTH * 2)

#define MODE_DESC_OFFSET LINE_BIG(NUM_OPTIONS_MODE_SELECT) + LINE(1) + NUM_OPTIONS_MODE_SELECT+2

void render_base_options(const char* options[], const int sizes[], const uint16_t colours[], int num_options, uint8_t yOffset)
{
    for (uint8_t i = 0; i < num_options; i++)
    {
        if (i == s_selected_option)
            print_text(">", 1, 2, 0, LINE_BIG(i) + yOffset + i*2, 0xffff, 0x0000);

        print_text(options[i], sizes[i], 2, CHARS(1), LINE_BIG(i) + yOffset + i*2, colours[i], 0x0000);
    }
}

void SCENE_F(menu, render)()
{
    fill_rect(0, 0, SCREEN_W, SCREEN_H, 0x0000);

    switch (s_status)
    {
    case STATUS_HOME_MENU:
        render_base_options(s_home_menu_options, s_home_menu_option_sizes, s_home_menu_option_colours, NUM_OPTIONS_HOME_MENU, 0x0);

        break;
    case STATUS_MODE_SELECT:
        render_base_options(s_mode_select_options, s_mode_select_option_sizes, s_mode_select_option_colours, NUM_OPTIONS_MODE_SELECT, 0x0);

        switch (s_selected_option)
        {
        case 0: // Easy
            print_text("Lots of coins,", 14, 1, 0, MODE_DESC_OFFSET, 0xffff, 0x0000);
            print_text("lots of time!", 13, 1, 0, MODE_DESC_OFFSET+LINE(1)+2, 0xffff, 0x0000);

            break;
        case 1: // Medium
            print_text("Reduced coins,", 14, 1, 0, MODE_DESC_OFFSET, 0xffff, 0x0000);
            print_text("lots of time!", 13, 1, 0, MODE_DESC_OFFSET+LINE(1)+2, 0xffff, 0x0000);

            break;
        case 2: // Hard
            print_text("Reduced coins,", 14, 1, 0, MODE_DESC_OFFSET, 0xffff, 0x0000);
            print_text("reduced time!", 13, 1, 0, MODE_DESC_OFFSET+LINE(1)+2, 0xffff, 0x0000);

            break;
        case 3: // Endless
            print_text("Reduced coins,", 14, 1, 0, MODE_DESC_OFFSET, 0xffff, 0x0000);
            print_text("no time limit!", 14, 1, 0, MODE_DESC_OFFSET+LINE(1)+2, 0xffff, 0x0000);

            break;
        case 4: // Random
            print_text("Random coins,", 13, 1, 0, MODE_DESC_OFFSET, 0xffff, 0x0000);
            print_text("random time limit!", 18, 1, 0, MODE_DESC_OFFSET+LINE(1)+2, 0xffff, 0x0000);
            
            break;
        case 5: // Return
            print_text("Return to menu", 14, 1, 0, MODE_DESC_OFFSET, 0xffff, 0x0000);

            break;
        default:
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

    SCENE_F(menu, render)();
}

void step_base_options(const input_status* const input, uint8_t num_options)
{
    if (input->trigger & BUTTON_UP)
    {
        if (s_selected_option == 0)
            s_selected_option = num_options - 1;
        else
            --s_selected_option;

        render_menu_scene();
    } else if (input->trigger & BUTTON_DOWN)
    {
        if (s_selected_option == num_options - 1)
            s_selected_option = 0;
        else
            ++s_selected_option;

        render_menu_scene();
    }
}

void home_menu_on_click()
{
    if (s_selected_option == 0)
        s_status = STATUS_MODE_SELECT;
    else if (s_selected_option == 1)
        s_status = STATUS_ABOUT;

    s_selected_option = 0;

    render_menu_scene();
}

void mode_select_on_click()
{
    switch (s_selected_option)
    {
    case 5: // Return
        s_status = STATUS_HOME_MENU;

        s_selected_option = 0;
        render_menu_scene();

        break;
    default:
        change_scene(SCENE(game));

        set_infobox_start_time(15);
        set_infobox_coins(1);

        break;
    }
}

void SCENE_F(menu, step)(const input_status* const input)
{
    switch (s_status)
    {
    case STATUS_HOME_MENU:
        step_base_options(input, NUM_OPTIONS_HOME_MENU);

        if (input->trigger & BUTTON_ENTER)
            home_menu_on_click();

        break;
    case STATUS_MODE_SELECT:
        step_base_options(input, NUM_OPTIONS_MODE_SELECT);

        if (input->trigger & BUTTON_ENTER)
            mode_select_on_click();

        break;
    case STATUS_ABOUT:
        if (input->trigger & BUTTON_ENTER)
        {
            s_selected_option = 0;
            s_status = STATUS_HOME_MENU;

            render_menu_scene();
        }
    
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
