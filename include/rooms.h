/**
 * @file: rooms.h
 * @desc: Room system interface - procedural generation and collision detection
 * @auth: James Farrelly (C24402114)
 */

#ifndef ROOMS_H_
#define ROOMS_H_

#include "character.h"
#include <stdint.h>
#include <stdbool.h>

#define ROOM_SIZE_X 3
#define ROOM_SIZE_Y 3

/**
 * 3x3 grid representing room tile layout
 */
typedef uint8_t room_structure_t[ROOM_SIZE_X][ROOM_SIZE_Y];

/**
 * Simple 2D coordinate pair
 */
typedef struct coordinate_set_2x2
{
    uint8_t x, y;                   /* X and Y coordinates */
} coordinate_set_2x2_t;

#define MAX_TRANSITION_POINTS    4 /* Max number of transition points */
#define MAX_WALL_COLLISION_SETS  3 /* Max number of collision sets for additional walls. */
#define MAX_TRANS_COLLISION_SETS 4 /* Max number of collision sets for transition points */

typedef const coordinate_set_2x2_t const_coordinate_set_2x2_t;

/**
 * Room structure containing layout and collision data
 */
typedef struct room {
    uint8_t index;                  /* Room index identifier */
    room_structure_t structure;     /* 3x3 tile grid layout */
    
    /*
     * Transition points to adjacent rooms
     * Index: 0=top, 1=left, 2=right, 3=bottom
     * Complement: 0=3, 1=2, 2=1, 3=0
     */
    int8_t transition_points[MAX_TRANSITION_POINTS];
    
    uint8_t wall_collision_set_len; /* Number of wall collision areas */
    coordinate_set_2x2_t wall_collision_sets[MAX_WALL_COLLISION_SETS]; /* Wall collision areas (max 3 can be present) */
    
    uint8_t trans_collision_set_len; /* Number of transition collision areas */
    coordinate_set_2x2_t trans_collision_sets[MAX_TRANS_COLLISION_SETS]; /* Transition collision areas */
} room;

/**
 * Initialize the room system and generate initial room
 */
void init_rooms(void);

/**
 * Update room state and handle transitions
 */
void step_rooms(void);

/**
 * Renders the current room
 */
void render_current_room(void);

/**
 * Check if character is colliding with any walls
 * @return true if collision detected, false otherwise
 */
bool colliding_with_walls(const character* const player);

/**
 * Sets the generation chance of a coin.
 *
 * @param percentage The percentage.
 */
void set_coin_generation_chance(uint8_t percentage);

#endif // !ROOMS_H_
