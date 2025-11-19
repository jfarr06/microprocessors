/**
 * @file: rooms.h
 * @desc: Room system interface - procedural generation and collision detection
 * @auth: James Farrelly (C24402114)
 */

#ifndef ROOMS_H_
#define ROOMS_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * 3x3 grid representing room tile layout
 */
typedef uint8_t room_structure_t[3][3];

/**
 * Coordinate set defining a rectangular area with optional image data
 */
typedef struct coordinate_set
{
    uint8_t x, y, w, h;             /* Position and dimensions */
    const uint16_t* img_data;       /* Optional image data pointer */
} coordinate_set_t;

/**
 * Simple 2D coordinate pair
 */
typedef struct coordinate_set_2x2
{
    uint8_t x, y;                   /* X and Y coordinates */
} coordinate_set_2x2_t;

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
    struct room* transition_points[4];
    
    uint8_t wall_collision_set_len; /* Number of wall collision areas */
    
    /*
     * Wall collision areas (max 3 can be present)
     */
    coordinate_set_t wall_collision_sets[3];
    
    uint8_t trans_collision_set_len; /* Number of transition collision areas */
    coordinate_set_t trans_collision_sets[4]; /* Transition collision areas */
} room;

/* Maximum number of rooms that can be generated */
#define MAX_NUM_ROOMS 5

/* Room tile state values */
#define ROOM_TILE_STATE_WALL 0x0    /* Impassable wall tile */
#define ROOM_TILE_STATE_HALL 0x1    /* Walkable hallway tile */
#define ROOM_TILE_STATE_TRAN 0x2    /* Transition point to another room */
#define ROOM_TILE_STATE_COIN 0x3    /* Collectible coin tile */

/**
 * Initialize the room system and generate initial room
 */
void init_rooms(void);

/**
 * Update room state and handle transitions
 */
void step_rooms(void);

/**
 * Check if character is colliding with any walls
 * @return true if collision detected, false otherwise
 */
bool colliding_with_walls(void);

#endif // !ROOMS_H_