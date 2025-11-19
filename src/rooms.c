/**
 * @file: rooms.c
 * @desc: Room system implementation - procedural generation and collision detection
 * @auth: James Farrelly (C24402114)
 */

#include <stdlib.h>
#include <memory.h>

#include <rand.h>
#include <rooms.h>
#include <util.h>
#include <display.h>
#include <infobox.h>
#include <character.h>
#include <images_data.h>

room* rooms[MAX_NUM_ROOMS] = {0};
uint8_t room_index = 0;

#pragma pack(push, 1)
static const room_structure_t default_room_template = 
{
    // # ▲ #
    //◀   ▶
    // # ▼ #
    {ROOM_TILE_STATE_WALL, ROOM_TILE_STATE_TRAN, ROOM_TILE_STATE_WALL},
    {ROOM_TILE_STATE_TRAN, ROOM_TILE_STATE_HALL, ROOM_TILE_STATE_TRAN},
    {ROOM_TILE_STATE_WALL, ROOM_TILE_STATE_TRAN, ROOM_TILE_STATE_WALL}
};

static const coordinate_set_t wall_coordinates[3][3] = 
{
    {{0, 0, IMG_wall_width, IMG_wall_height, IMG_wall_data}, {42, 0, IMG_wall_width, IMG_wall_height, IMG_wall_data}, {84, 0, IMG_wall_width, IMG_wall_height, IMG_wall_data}},
    {{0, 42, IMG_wall_width, IMG_wall_height, IMG_wall_data}, {0}, {84, 42, IMG_wall_width, IMG_wall_height, IMG_wall_data}},
    {{0, 84, IMG_wall_width, IMG_wall_height, IMG_wall_data}, {42, 84, IMG_wall_width, IMG_wall_height, IMG_wall_data}, {84, 84, IMG_wall_width, IMG_wall_height, IMG_wall_data}}
};

#define TRANS_W 1
#define TRANS_H 42

static const coordinate_set_t trans_coordinates[3][3] = 
{
    {{0}, {42, 0, TRANS_H, TRANS_W}, {0}},
    {{0, 42, TRANS_W, TRANS_H}, {0}, {84+TRANS_H, 42, TRANS_W, TRANS_H}},
    {{0}, {42, 84+TRANS_H, TRANS_H, TRANS_W}, {0}}
};

#define COIN_R 8
#define COIN_X (SCREEN_W/2)-(COIN_R/2)
#define COIN_Y (SCREEN_H/2)-(COIN_R*2)

static const uint8_t transition_complement_indexes[4] = {3, 2, 1, 0};
static const coordinate_set_2x2_t transition_index_to_room_structure_indices[4] = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};
#pragma pack(pop)

uint8_t get_transition_index_by_coordinates(coordinate_set_t coords)
{
    if (coords.x == trans_coordinates[0][1].x && coords.y == trans_coordinates[0][1].y) return 0;
    if (coords.x == trans_coordinates[1][0].x && coords.y == trans_coordinates[1][0].y) return 1;
    if (coords.x == trans_coordinates[1][2].x && coords.y == trans_coordinates[1][2].y) return 2;
    if (coords.x == trans_coordinates[2][1].x && coords.y == trans_coordinates[2][1].y) return 3;

    return 0;
}

void render_current_room()
{
    fill_rect(0, 0, SCREEN_W, SCREEN_W, 0x0000); // Clear

    room* current_room = rooms[room_index];

    for (uint8_t row = 0; row < 3; row++)
    {
        for (uint8_t col = 0; col < 3; col++)
        {
            uint8_t state = current_room->structure[row][col];
            coordinate_set_t coords = wall_coordinates[row][col];

            switch (state)
            {
            case ROOM_TILE_STATE_WALL:
                coords = wall_coordinates[row][col];

                put_image(coords.x, coords.y, coords.w, coords.h, coords.img_data, 0, 0);
                
                break;
            case ROOM_TILE_STATE_COIN:

                fill_circle(COIN_X, COIN_Y, COIN_R, RGB16(0xff, 0xfb, 0x00));

                break;
            default:
                break;
            }
        }
    }
}

void init_rooms()
{
    if (rooms[0] != NULL)
    {
        for (uint8_t i = 0; i < MAX_NUM_ROOMS; i++)
        {
            if (rooms[i]) free(rooms[i]);
        }

        room_index = 0;
    }

    room* initial_room = (room*)malloc(sizeof(room));
    memcpy(initial_room->structure, default_room_template, sizeof(room_structure_t));
    memset(initial_room->wall_collision_sets, 0x0, sizeof(coordinate_set_t)*3);
    memset(initial_room->transition_points, 0x0, sizeof(room*)*4);
    initial_room->wall_collision_set_len = 0;

    initial_room->index = 0;
    initial_room->trans_collision_set_len = 4;
    initial_room->trans_collision_sets[0] = trans_coordinates[0][1];
    initial_room->trans_collision_sets[1] = trans_coordinates[1][0];
    initial_room->trans_collision_sets[2] = trans_coordinates[1][2];
    initial_room->trans_collision_sets[3] = trans_coordinates[2][1];

    rooms[room_index] = initial_room;

    render_current_room();
}

inline bool is_intersecting_coords(character* const self, coordinate_set_t coords)
{
    return rect_intersects_rect(self->x, self->y, self->width, self->height,
                                coords.x, coords.y, coords.w, coords.h);
}


bool colliding_with_normal_walls()
{
    character* player = get_character();

    return is_intersecting_coords(player, wall_coordinates[0][0]) ||
           is_intersecting_coords(player, wall_coordinates[0][2]) ||
           is_intersecting_coords(player, wall_coordinates[2][0]) ||
           is_intersecting_coords(player, wall_coordinates[2][2]);
}

bool colliding_with_walls()
{
    if (colliding_with_normal_walls()) return true;

    room* current_room = rooms[room_index];
    if (current_room->wall_collision_set_len == 0) return false;

    character* player = get_character();

    for (uint8_t i = 0; i < current_room->wall_collision_set_len; i++)
    {
        coordinate_set_t coords = current_room->wall_collision_sets[i];

        if (is_intersecting_coords(player, coords)) return true;
    }

    return false;
}

int8_t colliding_with_room_transition()
{
    room* current_room = rooms[room_index];
    character* player = get_character();

    for (uint8_t i = 0; i < current_room->trans_collision_set_len; i++)
    {
        coordinate_set_t coords = current_room->trans_collision_sets[i];

        if (is_intersecting_coords(player, coords)) return get_transition_index_by_coordinates(coords);
    }

    return -1;
}

// Example usage with your coin defines:
bool char_intersects_coin()
{
    character* player = get_character();

    return rect_intersects_circle(player->x, player->y, player->width, player->height,
                                  COIN_X, COIN_Y, COIN_R);
}

#define STATE_WALL_PERCENT 42.0f
#define STATE_TRAN_PERCENT 35.0f
#define STATE_COIN_PERCENT 23.0f

int get_random_state(bool use_coins)
{
    float rand_percent = sys_rand(0, 10000) / 100.0f;
    
    if (rand_percent < STATE_WALL_PERCENT) 
        return ROOM_TILE_STATE_WALL;
    if (rand_percent < STATE_WALL_PERCENT + STATE_TRAN_PERCENT) 
        return ROOM_TILE_STATE_TRAN;
    if (rand_percent < STATE_WALL_PERCENT + STATE_TRAN_PERCENT + STATE_COIN_PERCENT) 
        return use_coins ? ROOM_TILE_STATE_COIN : ROOM_TILE_STATE_TRAN;
    
    return ROOM_TILE_STATE_WALL;
}

void add_room(uint8_t transition_idx, room* transitioning_room)
{
    if (room_index == MAX_NUM_ROOMS-1)
        room_index = 0;
    else 
        room_index++;

    // Clean up of existing room.
    room* existing_room;
    if ((existing_room = rooms[room_index]) == NULL)
        existing_room = (room*)malloc(sizeof(room));

    room* new_room = existing_room;
    room_structure_t room_structure;
    memcpy(room_structure, default_room_template, sizeof(room_structure_t));
    memset(new_room->wall_collision_sets, 0x0, sizeof(coordinate_set_t)*3);
    memset(new_room->trans_collision_sets, 0x0, sizeof(coordinate_set_t)*4);
    memset(new_room->transition_points, 0x0, sizeof(room*)*4);

    new_room->index = room_index;
    new_room->wall_collision_set_len = 0;
    new_room->trans_collision_set_len = 1;

    uint8_t complement_transition_idx = transition_complement_indexes[transition_idx];
    transitioning_room->transition_points[transition_idx] = new_room;
    new_room->transition_points[complement_transition_idx] = transitioning_room;
    coordinate_set_2x2_t transition_indices = transition_index_to_room_structure_indices[complement_transition_idx];
    new_room->trans_collision_sets[0] = trans_coordinates[transition_indices.x][transition_indices.y];

    for (uint8_t row = 0; row < 3; row++)
    {
        for (uint8_t col = 0; col < 3; col++)
        {
            if (default_room_template[row][col] == ROOM_TILE_STATE_TRAN && !(row == transition_indices.x && col == transition_indices.y))
            {
                uint8_t state = get_random_state(false);
                if (state != ROOM_TILE_STATE_COIN) {
                    room_structure[row][col] = state;

                    if (state == ROOM_TILE_STATE_TRAN)
                    {
                        new_room->trans_collision_set_len++;
                        new_room->trans_collision_sets[new_room->trans_collision_set_len-1] = trans_coordinates[row][col];

                        continue;
                    }

                    if (state == ROOM_TILE_STATE_WALL)
                    {
                        new_room->wall_collision_set_len++;
                        new_room->wall_collision_sets[new_room->wall_collision_set_len-1] = wall_coordinates[row][col];

                        continue;
                    }
                }
            } else if (default_room_template[row][col] == ROOM_TILE_STATE_HALL)
            {
                uint8_t state = get_random_state(true);

                if (state == ROOM_TILE_STATE_COIN)
                {
                    room_structure[row][col] = state;
                }
            }
            
            // ===== Double check here because the state could have technically changed.
        }
    }

    memcpy(new_room->structure, room_structure, sizeof(room_structure_t));

    rooms[room_index] = new_room;
}

void step_rooms()
{
    room* current_room = rooms[room_index];
    int8_t idx;

    if (current_room->structure[1][1] == ROOM_TILE_STATE_COIN)
    {
        if (char_intersects_coin())
        {
            current_room->structure[1][1] = ROOM_TILE_STATE_HALL;
            
            inc_infobox_coins();
            fill_circle(COIN_X, COIN_Y, COIN_R, 0x0000);
            render_character();
        }
    }

    if ((idx = colliding_with_room_transition()) >= 0)
    {
        character* player = get_character();

        player->oldx = player->x, player->oldy = player->y;

        room* transition_room = current_room->transition_points[idx];

        uint8_t complement_idx = transition_complement_indexes[idx];
        if (complement_idx == 0)
            player->y = 5;
        else if (complement_idx == 1)
            player->x = 5;
        else if (complement_idx == 2)
            player->x = SCREEN_W-player->width-5;
        else if (complement_idx == 3)
            player->y = SCREEN_W-player->height-5;

        if (!transition_room)
            add_room(idx, current_room);
        else
            room_index = transition_room->index;

        render_character();
        render_current_room();
    }
}