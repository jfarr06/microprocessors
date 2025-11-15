/**
 * @file: main.c
 * @desc: Game entrypoint
 * @auth: James Farrelly (C24402114)
 */

#include <stdarg.h>
#include <stdint.h>
#include <stm32f031x6.h>

#include <io.h>
#include <rand.h>
#include <notes.h>
#include <debug.h>
#include <images.h>
#include <display.h>

#include <rooms.h>
#include <sprites.h>
#include <infobox.h>

#include <nucleo_f031k6/adc.h>
#include <nucleo_f031k6/sound.h>
#include <nucleo_f031k6/clock.h>
#include <nucleo_f031k6/serial.h>
#include <nucleo_f031k6/common.h>

int main()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN; 

    init_pll();
    init_systick();
    init_adc();
    init_rand();

    DBG_INIT();
    DBG_INFO("Initializing...");

    init_sound();
    init_display();
    init_io();

    init_rooms();
    init_sprites();
    init_infobox();

    DBG_INFO("Done initializing");

    bool playing_sound = false;

    uint16_t speed_factor = 50;

    while (1)
    {
        if (false)
        {
            if (playing_sound)
            {
                play_sound(0);
                playing_sound = false;
            } else {
                if (spr_frm % (speed_factor * 3) == 0)
                    play_sound(D5);
                else if (spr_frm % (speed_factor * 4) == 0)
                    play_sound(F3);
                else if (spr_frm % (speed_factor * 5) == 0)
                    play_sound(C2);
                else
                    play_sound(E5);

                playing_sound = true;
            }
        }

        if (button_enter_pressed())
        {
            set_infobox_coins(spr_frm);
        }

        step_rooms();
        sprites_step();
        step_infobox();
        
        sys_delay(1);
        spr_frm++;
    }
}
