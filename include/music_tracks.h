/**
 * @file: music_tracks.h
 * @desc: Predefined music tracks for background music
 * @auth: James Farrelly (C24402114)
 */

#ifndef MUSIC_TRACKS_H_
#define MUSIC_TRACKS_H_

#include <music.h>

/**
 * Get the menu background music track
 */
const music_track* get_menu_music(void);

/**
 * Get the game background music track
 */
const music_track* get_game_music(void);

#endif // MUSIC_TRACKS_H_
