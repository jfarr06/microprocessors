/**
 * @file: infobox.c
 * @desc: Information box (HUD) implementation - displays coins and timer
 * @auth: James Farrelly (C24402114)
 */

#include <math.h>
#include <nucleo_f031k6/clock.h>

#include <font5x7.h>
#include <display.h>

#include <infobox.h>

#define INFOBOX_BOUNDS_X 0
#define INFOBOX_BOUNDS_Y 128
#define INFOBOX_BOUNDS_W 127
#define INFOBOX_BOUNDS_H 31

#define COINS_OFFSET_X (INFOBOX_BOUNDS_X+5)
#define TIME_TITLE_OFFSET_X (INFOBOX_BOUNDS_W-((FONT_WIDTH + 2)*4)-5)
#define TIME_S_OFFSET_X (INFOBOX_BOUNDS_W-(FONT_WIDTH*2)-5)

#define STATUS_OFFSET_X ((INFOBOX_BOUNDS_W / 2) - FONT_WIDTH)

#define TITLE_POS_Y (INFOBOX_BOUNDS_Y+3)
#define VALUE_POS_Y ((INFOBOX_BOUNDS_Y+INFOBOX_BOUNDS_H/2-FONT_HEIGHT/2)+2)

bool did_change = true;
uint8_t infobox_coins_count = 0, infobox_coins_target = 0;
uint16_t infobox_time = 0, infobox_time_tick = 0;

uint8_t digit_count(uint16_t n)
{
    if (n == 0) return 1;

    return (uint8_t)log10(n) + 1;
}

void render_infobox_data()
{
    if (did_change) 
    {
        fill_rect(INFOBOX_BOUNDS_X+1, INFOBOX_BOUNDS_Y+FONT_HEIGHT+3, INFOBOX_BOUNDS_W-1, INFOBOX_BOUNDS_H-(FONT_HEIGHT+3)-1, 0x0000);

        uint8_t coins_digits = digit_count(infobox_coins_count);
        uint8_t coins_seperator_placement = (((FONT_WIDTH * 2) + 2) * coins_digits) + 5;
        uint8_t coins_target_placement = (((FONT_WIDTH * 2) + 2) * (coins_digits + 1)) + 5;
        uint16_t coins_colour = infobox_coins_count < infobox_coins_target ? RGB16(0xff, 0x00, 0x00) : RGB16(0x00, 0xff, 0x00);

        print_number(infobox_coins_count, 2, COINS_OFFSET_X, VALUE_POS_Y, coins_colour, 0x0000);
        print_text("/", 1, 2, coins_seperator_placement, VALUE_POS_Y, 0xffff, 0x0000);
        print_number(infobox_coins_target, 2, coins_target_placement, VALUE_POS_Y, RGB16(0xff, 0xfb, 0x00), 0x0000);

        if (infobox_time_tick != 0 && infobox_coins_count >= infobox_coins_target)
        {
            print_text("W", 1, 2, STATUS_OFFSET_X, VALUE_POS_Y, RGB16(0x00, 0xff, 0x00), 0x0000);
        } else if (infobox_time_tick == 0 && infobox_coins_count < infobox_coins_target)
        {
            print_text("L", 1, 2, STATUS_OFFSET_X, VALUE_POS_Y, RGB16(0xff, 0x00, 0x00), 0x0000);
        }

        uint8_t time_digits = digit_count(infobox_time_tick)+1;
        uint8_t time_placement = (((FONT_WIDTH * 2) + 2) * time_digits) + 5;
        uint16_t time_colour = infobox_time_tick == 0 ? RGB16(0xff, 0x00, 0x00) : 0xffff;

        print_number(infobox_time_tick, 2, INFOBOX_BOUNDS_W-time_placement, VALUE_POS_Y, time_colour, 0x0000);
        print_text("s", 1, 2, TIME_S_OFFSET_X, VALUE_POS_Y, 0xffff, 0x0000);
    
        did_change = false;
    }
}

void init_infobox()
{
    draw_rect(INFOBOX_BOUNDS_X, INFOBOX_BOUNDS_Y, INFOBOX_BOUNDS_W, INFOBOX_BOUNDS_H, 0xffff);
    print_text("Coins", 5, 1, COINS_OFFSET_X, TITLE_POS_Y, 0xffff, 0x0000);
    print_text("Time", 4, 1, TIME_TITLE_OFFSET_X, TITLE_POS_Y, 0xffff, 0x0000);

    infobox_time_tick = infobox_time;
    infobox_coins_count = 0;

    did_change = true;
}

void step_infobox()
{
    // Always count down.
    if (infobox_time_tick > 0 && infobox_coins_count < infobox_coins_target)
    {
        if (s_nucleo_f031k6_millis % 1000 == 0) // Every 1000ms (1s)
        {
            infobox_time_tick--;

            did_change = true;
        }
    }

    render_infobox_data();
}

void set_infobox_coins(uint8_t coins)
{
    infobox_coins_target = coins;

    did_change = true;
}

void set_infobox_start_time(uint16_t time)
{
    infobox_time = time;
    infobox_time_tick = time;

    did_change = true;
}

void inc_infobox_coins()
{
    infobox_coins_count++;

    did_change = true;
}