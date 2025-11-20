/**
 * @file: music_tracks.c
 * @desc: Predefined music tracks for background music
 * @auth: James Farrelly (C24402114)
 */

#include <music_tracks.h>
#include <notes.h>

/* Menu music - upbeat, welcoming melody */
static const music_note menu_notes[] = {
    {C5, 400}, {E5, 400}, {G5, 400}, {C6, 400},
    {G5, 400}, {E5, 400}, {C5, 800},
    {D5, 400}, {F5, 400}, {A5, 400}, {D6, 400},
    {A5, 400}, {F5, 400}, {D5, 800}
};

static const music_track menu_track = {
    .notes = menu_notes,
    .length = sizeof(menu_notes) / sizeof(music_note),
    .loop = 1
};

/* Game music - adventurous, energetic melody */
static const music_note game_notes[] = {
    {E4, 300}, {E4, 300}, {0, 150}, {E4, 300},
    {0, 150}, {C4, 300}, {E4, 600},
    {G4, 600}, {0, 300}, {G3, 600},
    {C4, 450}, {0, 150}, {G3, 450}, {0, 150},
    {E3, 450}, {0, 150}, {A3, 450}, {B3, 450},
    {AS3, 300}, {A3, 600}
};

static const music_track game_track = {
    .notes = game_notes,
    .length = sizeof(game_notes) / sizeof(music_note),
    .loop = 1
};

const music_track* get_menu_music(void)
{
    return &menu_track;
}

const music_track* get_game_music(void)
{
    return &game_track;
}
