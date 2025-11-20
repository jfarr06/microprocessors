/**
 * @file: rooms.c
 * @desc: Room system implementation - procedural generation and collision detection
 * @auth: James Farrelly (C24402114)
 */

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

#include <notes.h>
#include <music.h>

#include <rand.h>
#include <rooms.h>
#include <util.h>
#include <scenes.h>
#include <font5x7.h>
#include <colours.h>
#include <display.h>
#include <infobox.h>
#include <character.h>
#include <images_data.h>

/* Maximum number of rooms that can be generated */
#define MAX_NUM_ROOMS 3

/* Room tile state values */
#define ROOM_TILE_STATE_WALL 0x0    /* Impassable wall tile */
#define ROOM_TILE_STATE_HALL 0x1    /* Walkable hallway tile */
#define ROOM_TILE_STATE_TRAN 0x2    /* Transition point to another room */
#define ROOM_TILE_STATE_COIN 0x3    /* Collectible coin tile */

#define TRANS_W 1
#define TRANS_H 42

#define COIN_R      8
#define COIN_X      (SCREEN_W/2)-(COIN_R/2)
#define COIN_Y      (SCREEN_H/2)-(COIN_R*2)
#define COIN_COLOUR COLOUR_GOLD

#define TRANS_POINT_TOP    0
#define TRANS_POINT_LEFT   1
#define TRANS_POINT_RIGHT  2
#define TRANS_POINT_BOTTOM 3

#define TRANS_COORDINATES_TOP    s_trans_coordinates[0][1]
#define TRANS_COORDINATES_LEFT   s_trans_coordinates[1][0]
#define TRANS_COORDINATES_RIGHT  s_trans_coordinates[1][2]
#define TRANS_COORDINATES_BOTTOM s_trans_coordinates[2][1]

static const room_structure_t s_default_room_template = 
{
    // # ▲ #
    // ◀   ▶
    // # ▼ #
    {ROOM_TILE_STATE_WALL, ROOM_TILE_STATE_TRAN, ROOM_TILE_STATE_WALL},
    {ROOM_TILE_STATE_TRAN, ROOM_TILE_STATE_HALL, ROOM_TILE_STATE_TRAN},
    {ROOM_TILE_STATE_WALL, ROOM_TILE_STATE_TRAN, ROOM_TILE_STATE_WALL}
};

// All possible walls
static const coordinate_set_2x2_t s_wall_coordinates[ROOM_SIZE_X][ROOM_SIZE_Y] = 
{
    // ####
    // #  #
    // ####
    {{0x00, 0x00}, {0x2A, 0x00}, {0x54, 0x00}},
    {{0x00, 0x2A}, {0x00, 0x00}, {0x54, 0x2A}},
    {{0x00, 0x54}, {0x2A, 0x54}, {0x54, 0x54}}
};

// All possibles transitions
static const coordinate_set_2x2_t s_trans_coordinates[ROOM_SIZE_X][ROOM_SIZE_Y] = 
{
    //   ▲
    // ◀   ▶
    //   ▼ 
    {{0x00, 0x00}, {0x2A, 0x00}, {0x00, 0x00}},
    {{0x00, 0x2A}, {0x00, 0x00}, {0x7E, 0x2A}},
    {{0x00, 0x00}, {0x2A, 0x7E}, {0x00, 0x00}}
};

static const uint8_t s_complement_transition_indices[4] = {TRANS_POINT_BOTTOM, TRANS_POINT_RIGHT, TRANS_POINT_LEFT, TRANS_POINT_TOP};
static const coordinate_set_2x2_t s_transition_index_to_room_structure_indices[4] = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};
static const coordinate_set_2x2_t s_transition_point_dimensions[2] = {{TRANS_H, TRANS_W}, {TRANS_W, TRANS_H}};

static room* s_rooms[MAX_NUM_ROOMS] = {0};
static uint8_t s_room_index = 0;
static uint8_t s_room_count = 1;
static uint8_t s_coin_chance = 0;

static uint8_t get_transition_index_by_coordinates(coordinate_set_2x2_t coords)
{
    if (coords.x == TRANS_COORDINATES_TOP.x && coords.y == TRANS_COORDINATES_TOP.y) return TRANS_POINT_TOP;
    if (coords.x == TRANS_COORDINATES_LEFT.x && coords.y == TRANS_COORDINATES_LEFT.y) return TRANS_POINT_LEFT;
    if (coords.x == TRANS_COORDINATES_RIGHT.x && coords.y == TRANS_COORDINATES_RIGHT.y) return TRANS_POINT_RIGHT;
    if (coords.x == TRANS_COORDINATES_BOTTOM.x && coords.y == TRANS_COORDINATES_BOTTOM.y) return TRANS_POINT_BOTTOM;

    return TRANS_POINT_TOP;
}

static coordinate_set_2x2_t get_transition_dimensions_by_transition_index(uint8_t idx)
{
    if (idx == 0 || idx == 3) return s_transition_point_dimensions[0];

    return s_transition_point_dimensions[1];
}

void render_current_room()
{
    fill_rect(0, 0, SCREEN_W, SCREEN_W, COLOUR_BLACK); // Clear

    room* current_room = s_rooms[s_room_index];

    for (uint8_t row = 0; row < ROOM_SIZE_X; row++)
    {
        for (uint8_t col = 0; col < ROOM_SIZE_Y; col++)
        {
            uint8_t state = current_room->structure[row][col];
            coordinate_set_2x2_t coords = s_wall_coordinates[row][col];

            switch (state)
            {
            case ROOM_TILE_STATE_WALL:
                coords = s_wall_coordinates[row][col];

                put_image(coords.x, coords.y, IMG_wall_width, IMG_wall_height, IMG_wall_data, 0);
                
                break;
            case ROOM_TILE_STATE_COIN:
                fill_circle(COIN_X, COIN_Y, COIN_R, COIN_COLOUR);

                break;
            default:
                break;
            }
        }
    }

    uint8_t room_digits = digit_count(s_room_count);
    fill_rect(0, 0, CHARS(room_digits)+2*room_digits-1, FONT_HEIGHT*2, COLOUR_BLACK);
    print_number(s_room_count, 2, 0, 0, COLOUR_WHITE, COLOUR_BLACK);
}

void set_coin_generation_chance(uint8_t percentage)
{
    s_coin_chance = percentage;
}

void init_rooms()
{
    if (s_rooms[0] != NULL)
    {
        for (uint8_t i = 0; i < MAX_NUM_ROOMS; i++)
            if (s_rooms[i])
            {
                free(s_rooms[i]);
                s_rooms[i] = NULL;
            }

        s_room_index = 0;
    }

    room* initial_room = (room*)malloc(sizeof(room));
    memcpy(initial_room->structure, s_default_room_template, sizeof(room_structure_t));
    memset(initial_room->wall_collision_sets, 0x0, sizeof(coordinate_set_2x2_t)*MAX_WALL_COLLISION_SETS);
    memset(initial_room->transition_points, -1, sizeof(int8_t)*MAX_TRANSITION_POINTS);
    initial_room->wall_collision_set_len = 0;

    initial_room->index = 0;
    initial_room->trans_collision_set_len = MAX_TRANS_COLLISION_SETS;
    initial_room->trans_collision_sets[0] = TRANS_COORDINATES_TOP;
    initial_room->trans_collision_sets[1] = TRANS_COORDINATES_LEFT;
    initial_room->trans_collision_sets[2] = TRANS_COORDINATES_RIGHT;
    initial_room->trans_collision_sets[3] = TRANS_COORDINATES_BOTTOM;

    s_rooms[s_room_index] = initial_room;
    s_room_count = 1;
}

static bool is_intersecting_coords(const character* const player, coordinate_set_2x2_t coords, uint8_t w, uint8_t h)
{
    return rect_intersects_rect(player->x, player->y, player->width, player->height,
                                coords.x, coords.y, w, h);
}


static bool colliding_with_normal_walls(const character* const player)
{
    // Check corner walls (coordinates at [0][0], [0][2], [2][0], [2][2])
    static const uint8_t corner_positions[4][2] = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
    
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t row = corner_positions[i][0];
        uint8_t col = corner_positions[i][1];
        
        if (is_intersecting_coords(player, s_wall_coordinates[row][col], IMG_wall_width, IMG_wall_height))
            return true;
    }
    
    return false;
}

bool colliding_with_walls(const character* const player)
{
    if (colliding_with_normal_walls(player)) return true;

    room* current_room = s_rooms[s_room_index];
    if (current_room->wall_collision_set_len == 0) return false;


    for (uint8_t i = 0; i < current_room->wall_collision_set_len; i++)
    {
        coordinate_set_2x2_t coords = current_room->wall_collision_sets[i];

        if (is_intersecting_coords(player, coords, IMG_wall_width, IMG_wall_height)) return true;
    }

    return false;
}

static int8_t colliding_with_room_transition(const character* const player)
{
    room* current_room = s_rooms[s_room_index];

    for (uint8_t i = 0; i < current_room->trans_collision_set_len; i++)
    {
        coordinate_set_2x2_t coords = current_room->trans_collision_sets[i];
        uint8_t transition_index = get_transition_index_by_coordinates(coords);
        coordinate_set_2x2_t dimensions = get_transition_dimensions_by_transition_index(transition_index);

        if (is_intersecting_coords(player, coords, dimensions.x, dimensions.y)) return transition_index;
    }

    return -1;
}

// Example usage with your coin defines:
static bool char_intersects_coin(const character* const player)
{
    return rect_intersects_circle(player->x, player->y, player->width, player->height,
                                  COIN_X, COIN_Y, COIN_R);
}

#define STATE_WALL_PERCENT 52
#define STATE_TRAN_PERCENT 48

uint8_t get_random_state()
{
    uint8_t rand_percent = sys_rand(0, 100);
    
    if (rand_percent < STATE_WALL_PERCENT) 
        return ROOM_TILE_STATE_WALL;
    if (rand_percent < STATE_WALL_PERCENT + STATE_TRAN_PERCENT) 
        return ROOM_TILE_STATE_TRAN;
    
    return ROOM_TILE_STATE_WALL;
}

static void add_room(uint8_t transition_idx, room* const transitioning_room)
{
    if (s_room_index == MAX_NUM_ROOMS-1)
        s_room_index = 0;
    else 
        s_room_index++;

    s_room_count++;

    // Clean up of existing room.
    room* existing_room;
    if ((existing_room = s_rooms[s_room_index]) == NULL)
        existing_room = (room*)malloc(sizeof(room));

    room* new_room = existing_room;
    room_structure_t room_structure;
    memcpy(room_structure, s_default_room_template, sizeof(room_structure_t));
    memset(new_room->wall_collision_sets, 0x0, sizeof(coordinate_set_2x2_t)*MAX_WALL_COLLISION_SETS);
    memset(new_room->trans_collision_sets, 0x0, sizeof(coordinate_set_2x2_t)*MAX_TRANS_COLLISION_SETS);
    memset(new_room->transition_points, -1, sizeof(int8_t)*MAX_TRANSITION_POINTS);

    new_room->index = s_room_index;
    new_room->wall_collision_set_len = 0;
    new_room->trans_collision_set_len = 1;

    uint8_t complement_transition_idx = s_complement_transition_indices[transition_idx];
    transitioning_room->transition_points[transition_idx] = new_room->index;
    new_room->transition_points[complement_transition_idx] = transitioning_room->index;
    coordinate_set_2x2_t transition_indices = s_transition_index_to_room_structure_indices[complement_transition_idx];
    new_room->trans_collision_sets[0] = s_trans_coordinates[transition_indices.x][transition_indices.y];

    for (uint8_t row = 0; row < ROOM_SIZE_X; row++)
    {
        for (uint8_t col = 0; col < ROOM_SIZE_Y; col++)
        {
            if (s_default_room_template[row][col] == ROOM_TILE_STATE_TRAN && !(row == transition_indices.x && col == transition_indices.y))
            {
                uint8_t state = get_random_state();
                if (state != ROOM_TILE_STATE_COIN) {
                    room_structure[row][col] = state;

                    if (state == ROOM_TILE_STATE_TRAN)
                    {
                        new_room->trans_collision_set_len++;
                        new_room->trans_collision_sets[new_room->trans_collision_set_len-1] = s_trans_coordinates[row][col];

                        continue;
                    }

                    if (state == ROOM_TILE_STATE_WALL)
                    {
                        new_room->wall_collision_set_len++;
                        new_room->wall_collision_sets[new_room->wall_collision_set_len-1] = s_wall_coordinates[row][col];

                        continue;
                    }
                }
            } else if (s_default_room_template[row][col] == ROOM_TILE_STATE_HALL && rand_chance(s_coin_chance))
            {
                room_structure[row][col] = ROOM_TILE_STATE_COIN;
            }
        }
    }

    memcpy(new_room->structure, room_structure, sizeof(room_structure_t));

    s_rooms[s_room_index] = new_room;
}

void step_rooms()
{
    int8_t idx;
    character* const player = get_character();
    room* const current_room = s_rooms[s_room_index];

    if (current_room->structure[1][1] == ROOM_TILE_STATE_COIN)
    {
        if (char_intersects_coin(player))
        {
            current_room->structure[1][1] = ROOM_TILE_STATE_HALL;
            
            inc_infobox_coins();
            fill_circle(COIN_X, COIN_Y, COIN_R, COLOUR_BLACK);
            render_character();

            play_sound_effect(E5, 150);
        }
    } else if ((idx = colliding_with_room_transition(player)) >= 0) // Cannot collide with both a coin and a room transition at the same time.
    {
        player->oldx = player->x, player->oldy = player->y;

        int8_t transition_room = current_room->transition_points[idx];
        uint8_t complement_idx = s_complement_transition_indices[idx];
        switch (complement_idx) 
        {
            case TRANS_POINT_TOP:
                player->y = 5;

                break;
            case TRANS_POINT_LEFT:
                player->x = 5;
                
                break;
            case TRANS_POINT_RIGHT:
                player->x = SCREEN_W - player->width - 5;
                
                break;
            case TRANS_POINT_BOTTOM:
                player->y = SCREEN_W - player->height - 5;

                break;
        }
        
        if (transition_room == -1)
            add_room(idx, current_room);
        else
            s_room_index = transition_room;

        render_character();
        render_current_room();
    }
}
