/**
 * @file: music.c
 * @desc: Background music system with audio mixing implementation
 * @auth: James Farrelly (C24402114)
 */

#include <nucleo_f031k6/sound.h>
#include <nucleo_f031k6/clock.h>

#include <music.h>
#include <debug.h>

/* Current music state */
static const music_track* s_current_track = 0;
static uint16_t s_current_note_index = 0;
static uint32_t s_note_start_time = 0;

/* Sound effect state */
static uint32_t s_effect_frequency = 0;
static uint32_t s_effect_end_time = 0;

/* Tempo adjustment based on time remaining */
static uint16_t s_tempo_multiplier = 100; /* Percentage: 100 = normal, 150 = 1.5x speed */

void init_music(void)
{
    init_nucleo_f031k6_sound();

    s_current_track = 0;
    s_current_note_index = 0;
    s_note_start_time = 0;
    s_effect_frequency = 0;
    s_effect_end_time = 0;
    s_tempo_multiplier = 100;

    DBG_INFO("Music system initialized");
}

void play_music(const music_track* track)
{
    if (track == 0) return;

    s_current_track = track;
    s_current_note_index = 0;
    s_note_start_time = nucleo_f031k6_millis;

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
    uint32_t current_time = nucleo_f031k6_millis;

    /* Check if sound effect has finished */
    if (s_effect_frequency != 0 && current_time >= s_effect_end_time)
    {
        s_effect_frequency = 0;
        DBG_TRACE("Sound effect finished");
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
    
    /* Apply tempo multiplier to note duration */
    uint32_t adjusted_duration = (current_note->duration * 100) / s_tempo_multiplier;

    if (elapsed >= adjusted_duration)
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
                DBG_TRACE("Music looping");
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
    s_effect_end_time = nucleo_f031k6_millis + duration;
    
    play_nucleo_f031k6_sound(frequency);
    
    DBG_INFO("Playing sound effect: %dHz for %dms", frequency, duration);
}

void set_music_tempo_by_time(uint16_t time_remaining)
{
    /* Speed up music as time gets low:
     * - 30+ seconds: normal speed (100%)
     * - 20-29 seconds: slightly faster (110%)
     * - 10-19 seconds: faster (125%)
     * - 5-9 seconds: much faster (150%)
     * - 0-4 seconds: very fast (200%)
     * - 0 or no timer: normal speed (100%)
     */
    
    if (time_remaining == 0)
    {
        s_tempo_multiplier = 100; /* No timer, normal speed */
    }
    else if (time_remaining >= 30)
    {
        s_tempo_multiplier = 100;
    }
    else if (time_remaining >= 20)
    {
        s_tempo_multiplier = 110;
    }
    else if (time_remaining >= 10)
    {
        s_tempo_multiplier = 125;
    }
    else if (time_remaining >= 5)
    {
        s_tempo_multiplier = 150;
    }
    else
    {
        s_tempo_multiplier = 200;
    }
}
