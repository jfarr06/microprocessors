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

#define TITLE_POS_Y (INFOBOX_BOUNDS_Y+3)
#define VALUE_POS_Y ((INFOBOX_BOUNDS_Y+INFOBOX_BOUNDS_H/2-FONT_HEIGHT/2)+2)

bool did_change = false;
int infobox_coins_count = 0, infobox_time = 0;

int digit_count(int n)
{
    if (n == 0) return 1; // Zero has one digit
    return (int)log10(n) + 1;
}

void render_infobox_data()
{
    if (did_change) 
    {
        fill_rect(INFOBOX_BOUNDS_X+1, INFOBOX_BOUNDS_Y+FONT_HEIGHT+3, INFOBOX_BOUNDS_W-1, INFOBOX_BOUNDS_H-(FONT_HEIGHT+3)-1, 0x0000);

        // titles first

        int32_t digits = digit_count(infobox_time)+1;
        int32_t placement = (((FONT_WIDTH * 2) + 2) * digits) + 5;

        print_number(infobox_coins_count, 2, COINS_OFFSET_X, VALUE_POS_Y, rgb_to_word(0xff, 0xfb, 0x00), 0x0000);
        print_number(infobox_time, 2, INFOBOX_BOUNDS_W-placement, VALUE_POS_Y, 0xffff, 0x0000);
        print_text("s", 1, 2, TIME_S_OFFSET_X, VALUE_POS_Y, 0xffff, 0x0000);
    
        did_change = false;
    }
}

void init_infobox()
{
    draw_rect(INFOBOX_BOUNDS_X, INFOBOX_BOUNDS_Y, INFOBOX_BOUNDS_W, INFOBOX_BOUNDS_H, 0xffff);
    print_text("Coins", 5, 1, COINS_OFFSET_X, TITLE_POS_Y, 0xffff, 0x0000);
    print_text("Time", 4, 1, TIME_TITLE_OFFSET_X, TITLE_POS_Y, 0xffff, 0x0000);

    did_change = true;
    render_infobox_data();
}

void step_infobox()
{
    // Always count down.
    if (infobox_time > 0)
    {
        if (st_millis % 1000 == 0) // Every 1000ms (1s)
        {
            infobox_time--;

            did_change = true;
        }
    }

    render_infobox_data();
}

void set_infobox_coins(int coins)
{
    infobox_coins_count = coins;

    did_change = true;
}

void set_infobox_start_time(int time)
{
    infobox_time = time;

    did_change = true;
}