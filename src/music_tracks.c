/**
 * @file: music_tracks.c
 * @desc: Predefined music tracks for background music
 * @auth: James Farrelly (C24402114)
 */

#include <stdbool.h>

#include <notes.h>
#include <music_tracks.h>

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
    .loop = true
};

/* Game music - adventurous, energetic melody */
static const music_note game_notes[] = {
    {E4, 300}, {E4, 300}, {0, 150}, {E4, 300},
    {0, 150}, {C4, 300}, {E4, 600},
    {G4, 600}, {0, 300}, {G3, 600},
    {C4, 450}, {0, 150}, {G3, 450}, {0, 150},
    {E3, 450}, {0, 150}, {A3, 450}, {B3, 450},
    {AS3_Bb3, 300}, {A3, 600}
};

static const music_track game_track = {
    .notes = game_notes,
    .length = sizeof(game_notes) / sizeof(music_note),
    .loop = true
};

/* Victory music - triumphant, celebratory melody */
static const music_note victory_notes[] = {
    {C5, 200}, {C5, 200}, {C5, 200}, {C5, 600},
    {GS4_Ab4, 600}, {AS4_Bb4, 600},
    {C5, 200}, {AS4_Bb4, 200}, {C5, 800},
    {G5, 200}, {F5, 200}, {E5, 200}, {D5, 200},
    {C5, 1200}
};

static const music_track victory_track = {
    .notes = victory_notes,
    .length = sizeof(victory_notes) / sizeof(music_note),
    .loop = 0  /* Play once */
}
;
/* Defeat music - somber, descending melody */
static const music_note defeat_notes[] = {
    {C5, 400}, {B4, 400}, {AS4_Bb4, 400}, {A4, 400},
    {GS4_Ab4, 400}, {G4, 400}, {FS4_Gb4, 400}, {F4, 600},
    {0, 200},
    {F4, 300}, {E4, 300}, {DS4_Eb4, 600},
    {D4, 1200}
};

static const music_track defeat_track = {
    .notes = defeat_notes,
    .length = sizeof(defeat_notes) / sizeof(music_note),
    .loop = 0  /* Play once */
};

const music_track* get_menu_music(void)
{
    return &menu_track;
}

const music_track* get_game_music(void)
{
    return &game_track;
}

const music_track* get_victory_music(void)
{
    return &victory_track;
}

const music_track* get_defeat_music(void)
{
    return &defeat_track;
}
