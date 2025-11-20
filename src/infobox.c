/**
 * @file: infobox.c
 * @desc: Information box (HUD) implementation - displays coins and timer
 * @auth: James Farrelly (C24402114)
 */

#include "colours.h"
#include <nucleo_f031k6/clock.h>

#include <util.h>
#include <music.h>
#include <strings.h>
#include <font5x7.h>
#include <display.h>

#include <infobox.h>

#define INFOBOX_BOUNDS_X 0
#define INFOBOX_BOUNDS_Y 128
#define INFOBOX_BOUNDS_W 127
#define INFOBOX_BOUNDS_H 31

#define COINS_OFFSET_X      (INFOBOX_BOUNDS_X+5)
#define TIME_TITLE_OFFSET_X (INFOBOX_BOUNDS_W-((FONT_WIDTH + 2)*4)-5)
#define TIME_S_OFFSET_X     (INFOBOX_BOUNDS_W-(FONT_WIDTH*2)-5)
#define TIME_INF_OFFSET_X   (INFOBOX_BOUNDS_W-(FONT_WIDTH*2*3)-5)
#define STATUS_OFFSET_X     ((INFOBOX_BOUNDS_W / 2) - FONT_WIDTH)

#define TITLE_POS_Y (INFOBOX_BOUNDS_Y+3)
#define VALUE_POS_Y ((INFOBOX_BOUNDS_Y+INFOBOX_BOUNDS_H/2-FONT_HEIGHT/2)+2)

#define FONT_SPACER 5
#define FONTx2_BASE ((FONT_WIDTH * 2) + 2)

static bool s_should_render_infobox = true;
static uint8_t s_infobox_coins_count = 0, s_infobox_coins_target = 0;
static uint16_t s_infobox_time = 0, s_infobox_time_tick = 0;

static void s_try_render_infobox_data()
{
    if (s_should_render_infobox) 
    {
        fill_rect(INFOBOX_BOUNDS_X+1, INFOBOX_BOUNDS_Y+FONT_HEIGHT+3, INFOBOX_BOUNDS_W-1, INFOBOX_BOUNDS_H-(FONT_HEIGHT+3)-1, COLOUR_BLACK);

        uint8_t coins_digits = digit_count(s_infobox_coins_count);
        uint16_t coins_colour = s_infobox_coins_count < s_infobox_coins_target && s_infobox_coins_target != 0 ? COLOUR_RED : COLOUR_GREEN;
        print_number(s_infobox_coins_count, 2, COINS_OFFSET_X, VALUE_POS_Y, coins_colour, COLOUR_BLACK);
        
        if (s_infobox_coins_target != 0)
        {
            uint8_t coins_seperator_placement = (FONTx2_BASE * coins_digits) + FONT_SPACER;
            uint8_t coins_target_placement = (FONTx2_BASE * (coins_digits + 1)) + FONT_SPACER;

            print_text(STRING(INFOBOX_COINS_SEPARATOR), 2, coins_seperator_placement, VALUE_POS_Y, COLOUR_WHITE, COLOUR_BLACK);
            print_number(s_infobox_coins_target, 2, coins_target_placement, VALUE_POS_Y, COLOUR_GOLD, COLOUR_BLACK);
        }

        if (s_infobox_time != 0)
        {
            uint8_t time_digits = digit_count(s_infobox_time_tick)+1;
            uint8_t time_placement = (FONTx2_BASE * time_digits) + FONT_SPACER; 

            print_number(s_infobox_time_tick, 2, INFOBOX_BOUNDS_W-time_placement, VALUE_POS_Y, COLOUR_WHITE, COLOUR_BLACK);
            print_text(STRING(INFOBOX_TIME_SUFFIX_S), 2, TIME_S_OFFSET_X, VALUE_POS_Y, COLOUR_WHITE, COLOUR_BLACK);
        }
        else
            print_text(STRING(INFOBOX_TIME_INF), 2, TIME_INF_OFFSET_X, VALUE_POS_Y, COLOUR_WHITE, COLOUR_BLACK);
    
        s_should_render_infobox = false;
    }
}

static void s_render_infobox_header(void)
{
    draw_rect(INFOBOX_BOUNDS_X, INFOBOX_BOUNDS_Y, INFOBOX_BOUNDS_W, INFOBOX_BOUNDS_H, COLOUR_WHITE);
    print_text(STRING(INFOBOX_TITLE_COINS), 1, COINS_OFFSET_X, TITLE_POS_Y, COLOUR_WHITE, COLOUR_BLACK);
    print_text(STRING(INFOBOX_TITLE_TIME), 1, TIME_TITLE_OFFSET_X, TITLE_POS_Y, COLOUR_WHITE, COLOUR_BLACK); 
}

void render_infobox(void)
{
    s_render_infobox_header();

    s_should_render_infobox = true;
    s_try_render_infobox_data();
}

void init_infobox()
{
    s_infobox_time_tick = s_infobox_time;
    s_infobox_coins_count = 0;

    set_music_tempo_by_time(s_infobox_time_tick);
}

void step_infobox()
{
    // Always count down.
    if (s_infobox_time_tick > 0 && s_infobox_coins_count < s_infobox_coins_target)
    {
        if (nucleo_f031k6_millis % 1000 == 0) // Every 1000ms (1s)
        {
            s_infobox_time_tick--;

            set_music_tempo_by_time(s_infobox_time_tick);

            s_should_render_infobox = true;
        }
    }

    s_try_render_infobox_data();
}

void set_infobox_coins(uint8_t coins)
{
    s_infobox_coins_target = coins;

    s_should_render_infobox = true;
}

void set_infobox_start_time(uint16_t time)
{
    s_infobox_time = time;
    s_infobox_time_tick = time;

    s_should_render_infobox = true;
}

void inc_infobox_coins()
{
    s_infobox_coins_count++;

    s_should_render_infobox = true;
}

uint8_t get_running_status(void)
{
    if (s_infobox_time == 0) return RUNNING_STATUS_RUN;
    if (s_infobox_time_tick != 0 && s_infobox_coins_count >= s_infobox_coins_target) return RUNNING_STATUS_WIN;
    if (s_infobox_time_tick == 0 && s_infobox_coins_count < s_infobox_coins_target) return RUNNING_STATUS_LOSS;

    return RUNNING_STATUS_RUN;
}

inline uint16_t get_infobox_time_remaining(void)
{
    return s_infobox_time_tick;
}
