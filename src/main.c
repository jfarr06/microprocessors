/**
 * @file: main.c
 * @desc: Game entrypoint
 * @auth: James Farrelly (C24402114)
 */

#include <stdarg.h>
#include <stdint.h>
#include <stm32f031x6.h>

#include <io.h>
#include <debug.h>
#include <images.h>
#include <display.h>

#include <nucleo_f031k6/sound.h>
#include <nucleo_f031k6/clock.h>
#include <nucleo_f031k6/serial.h>
#include <nucleo_f031k6/common.h>

int main()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN; 

    init_pll();
    init_systick();

    DBG_INIT();
    DBG_TRACE("Initializing...");

    init_sound();
    init_display();
    init_io();

    DBG_TRACE("Done initializing");

    RENDER_IMAGE(dg1, 20, 80);

    //========== UPVARS
    
    int x = 50, y = 50;
    int oldx = x, oldy = y;
    int direction = 0;

    bool enter_pressed = false;

    while(1)
    {
        bool hmoved = false, vmoved = false;
        bool hinverted = false, vinverted = false;

        if (button_right_pressed())
        {
            if (x < 110)
            {
                x++;
                hmoved = true;
                hinverted = false;
            }
        }

        if (button_left_pressed())
        {
            if (x > 10)
            {
                x--;
                hmoved = true;
                hinverted = true;
            }
        }

        if (button_down_pressed())
        {
            if (y < 140)
            {
                y++;
                vmoved = true;
                vinverted = false;
            }
        }

        if (button_up_pressed())
        {
            if (y > 16)
            {
                y--;
                vmoved = true;
                vinverted = true;
            }
        }

        if (button_enter_pressed())
        {
            if (!enter_pressed)
            {
                print_text("Hi", 2, 2, 0, 0, 0xffff, 0x0000);

                enter_pressed = true;
            }
        }
        else {
            if (enter_pressed)
            {
                enter_pressed = false;

                print_text("Hi", 2, 2, 0, 0, 0x0000, 0x0000);
            }
        }

        if (vmoved || hmoved) // moved at all.
        {
            fill_rect(oldx, oldy, 12, 16, 0x0000);
            oldx = x, oldy = y;

            if (hmoved)
            {
                if (direction)
                    RENDER_IMAGE_ORIENTED(deco1, x, y, hinverted, 0);
                else
                    RENDER_IMAGE_ORIENTED(deco2, x, y, hinverted, 0);

                direction ^= 1;
            }
            else 
            {
                RENDER_IMAGE_ORIENTED(deco3, x, y, 0, vinverted);
            }
        }
    }
}
