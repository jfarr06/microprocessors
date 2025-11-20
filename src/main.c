/**
 * @file: main.c
 * @desc: Game entrypoint
 * @auth: James Farrelly (C24402114)
 */

#include <stdarg.h>
#include <stdint.h>
#include <stm32f031x6.h>

#include <util.h>
#include <rand.h>
#include <debug.h>
#include <music.h>
#include <display.h>

#include <scenes.h>

#include <nucleo_f031k6/sound.h>
#include <nucleo_f031k6/clock.h>
#include <nucleo_f031k6/common.h>

int main()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN; 

    init_nucleo_f031k6_pll();
    init_nucleo_f031k6_systick();
    init_rand();

    DBG_INIT();
    DBG_INFO("Initializing...");

    init_music();
    init_display();
    init_io();

    init_scenes();
    change_scene(SCENE(menu));

    DBG_INFO("Done initializing");

    while (1)
    {
        input_status input;
        read_latest_input(&input);
        
        step_scenes(&input);
        step_music();

        nucleo_f031k6_delay(1);
        game_frame++;
    }
}
