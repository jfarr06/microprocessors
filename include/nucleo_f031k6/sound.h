#ifndef NUCLEO_F031K6_SOUND_H_
#define NUCLEO_F031K6_SOUND_H_

#include <stm32f031x6.h>

/**
 * Initializes sound.
 */
void init_sound();

/**
 * Plays a tone at the specified frequency
 *
 * @param frequency The frequency.
 */
void play_sound(uint32_t frequency);

#endif
