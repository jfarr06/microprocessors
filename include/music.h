/**
 * @file: music.h
 * @desc: Background music system with audio mixing support
 * @auth: James Farrelly (C24402114)
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#include <stdint.h>

/**
 * Musical note entry in a melody
 * Contains frequency and duration information
 */
typedef struct {
    uint32_t frequency;  /* Frequency in Hz, 0 for rest/silence */
    uint16_t duration;   /* Duration in milliseconds */
} music_note;

/**
 * Music track definition
 * Contains a sequence of notes and metadata
 */
typedef struct {
    const music_note* notes; /* Pointer to array of notes */
    uint16_t length;         /* Number of notes in the track */
    uint8_t loop;            /* 1 to loop, 0 to play once */
} music_track;

/**
 * Initialize the music system
 */
void init_music(void);

/**
 * Play a music track in the background
 * 
 * @param track Pointer to the music track to play
 */
void play_music(const music_track* track);

/**
 * Stop the currently playing music
 */
void stop_music(void);

/**
 * Update the music system (should be called regularly)
 * This advances the music playback based on elapsed time
 */
void step_music(void);

/**
 * Play a sound effect (overrides background music temporarily)
 * 
 * @param frequency The frequency in Hz
 * @param duration Duration in milliseconds
 */
void play_sound_effect(uint32_t frequency, uint16_t duration);

/**
 * Set the time-based speed multiplier for music playback
 * Music speeds up as time runs low to create urgency
 * 
 * @param time_remaining Time remaining in seconds (0 = no timer)
 */
void set_music_tempo_by_time(uint16_t time_remaining);

#endif // MUSIC_H_
