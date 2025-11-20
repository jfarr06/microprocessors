/**
 * @file: music.c
 * @desc: Background music system with audio mixing implementation
 * @auth: James Farrelly (C24402114)
 */

#include <music.h>
#include <nucleo_f031k6/sound.h>
#include <nucleo_f031k6/clock.h>
#include <debug.h>

/* Current music state */
static const music_track* s_current_track = 0;
static uint16_t s_current_note_index = 0;
static uint32_t s_note_start_time = 0;

/* Sound effect state */
static uint32_t s_effect_frequency = 0;
static uint32_t s_effect_end_time = 0;

void init_music(void)
{
    s_current_track = 0;
    s_current_note_index = 0;
    s_note_start_time = 0;
    s_effect_frequency = 0;
    s_effect_end_time = 0;

    DBG_INFO("Music system initialized");
}

void play_music(const music_track* track)
{
    if (track == 0) return;

    s_current_track = track;
    s_current_note_index = 0;
    s_note_start_time = nucleo_f031k6_millis();

    DBG_INFO("Starting music track with %d notes", track->length);
}

void stop_music(void)
{
    s_current_track = 0;
    s_current_note_index = 0;
    
    /* Only stop sound if no effect is playing */
    if (s_effect_frequency == 0)
    {
        play_nucleo_f031k6_sound(0);
    }

    DBG_INFO("Music stopped");
}

void step_music(void)
{
    uint32_t current_time = nucleo_f031k6_millis();

    /* Check if sound effect has finished */
    if (s_effect_frequency != 0 && current_time >= s_effect_end_time)
    {
        s_effect_frequency = 0;
        DBG_DEBUG("Sound effect finished");
    }

    /* If a sound effect is playing, prioritize it */
    if (s_effect_frequency != 0)
    {
        return;
    }

    /* If no music is playing, ensure sound is off */
    if (s_current_track == 0)
    {
        play_nucleo_f031k6_sound(0);
        return;
    }

    /* Check if current note has finished */
    const music_note* current_note = &s_current_track->notes[s_current_note_index];
    uint32_t elapsed = current_time - s_note_start_time;

    if (elapsed >= current_note->duration)
    {
        /* Move to next note */
        s_current_note_index++;

        /* Check if we've reached the end of the track */
        if (s_current_note_index >= s_current_track->length)
        {
            if (s_current_track->loop)
            {
                /* Loop back to the beginning */
                s_current_note_index = 0;
                DBG_DEBUG("Music looping");
            }
            else
            {
                /* Stop the music */
                stop_music();
                return;
            }
        }

        /* Start the next note */
        current_note = &s_current_track->notes[s_current_note_index];
        s_note_start_time = current_time;
        play_nucleo_f031k6_sound(current_note->frequency);
    }
}

void play_sound_effect(uint32_t frequency, uint16_t duration)
{
    if (frequency == 0) return;

    s_effect_frequency = frequency;
    s_effect_end_time = nucleo_f031k6_millis() + duration;
    
    play_nucleo_f031k6_sound(frequency);
    
    DBG_DEBUG("Playing sound effect: %dHz for %dms", frequency, duration);
}
