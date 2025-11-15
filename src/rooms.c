#include <stdlib.h>
#include <memory.h>

#include <rand.h>
#include <rooms.h>
#include <display.h>
#include <sprites.h>
#include <infobox.h>
#include <images_data.h>

room* rooms[MAX_NUM_ROOMS] = {0};
int room_index = 0;

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
#pragma pack(pop)

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

static const coordinate_set_t hall_coordinates[3][3] =
{
    {{0}, {42, 0, IMG_wall_half_top_width, IMG_wall_half_top_height, IMG_wall_half_top_data}, {0}},
    {{0, 42, IMG_wall_half_side_width, IMG_wall_half_side_height, IMG_wall_half_side_data}, {0}, {84+IMG_wall_half_side_width, 42, IMG_wall_half_side_width, IMG_wall_half_side_height, IMG_wall_half_side_data}},
    {{0}, {42, 84+IMG_wall_half_top_height, IMG_wall_half_top_width, IMG_wall_half_top_height, IMG_wall_half_top_data}, {0}}
};

static const int transition_complement_indexes[4] = {3, 2, 1, 0};
static const coordinate_set_2x2_t transition_index_to_room_structure_indices[4] = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

int get_transition_index_by_coordinates(coordinate_set_t coords)
{
    if (coords.x == trans_coordinates[0][1].x && coords.y == trans_coordinates[0][1].y) return 0;
    if (coords.x == trans_coordinates[1][0].x && coords.y == trans_coordinates[1][0].y) return 1;
    if (coords.x == trans_coordinates[1][2].x && coords.y == trans_coordinates[1][2].y) return 2;
    if (coords.x == trans_coordinates[2][1].x && coords.y == trans_coordinates[2][1].y) return 3;

    return -1;
}

void render_current_room()
{
    fill_rect(0, 0, SCREEN_W, SCREEN_W, 0x0000); // Clear

    room* current_room = rooms[room_index];

    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
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

                fill_circle(COIN_X, COIN_Y, COIN_R, rgb_to_word(0xff, 0xfb, 0x00));

                break;
            case ROOM_TILE_STATE_HALL:
                coords = hall_coordinates[row][col];

                if (coords.w > 0 && coords.h > 0)
                    put_image(coords.x, coords.y, coords.w, coords.h, coords.img_data, 0, 0);

                break;
            default:
                break;
            }
        }
    }
}

void init_rooms()
{
    room* initial_room = (room*)malloc(sizeof(room));
    memcpy(initial_room->structure, default_room_template, sizeof(room_structure_t));
    memset(initial_room->wall_collision_sets, 0x0, sizeof(coordinate_set_t)*6);
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

bool lines_intersect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                     uint16_t x3, uint16_t y3, uint16_t x4, uint16_t y4)
{
    uint16_t min_x1 = x1 < x2 ? x1 : x2;
    uint16_t max_x1 = x1 > x2 ? x1 : x2;
    uint16_t min_y1 = y1 < y2 ? y1 : y2;
    uint16_t max_y1 = y1 > y2 ? y1 : y2;
    
    uint16_t min_x2 = x3 < x4 ? x3 : x4;
    uint16_t max_x2 = x3 > x4 ? x3 : x4;
    uint16_t min_y2 = y3 < y4 ? y3 : y4;
    uint16_t max_y2 = y3 > y4 ? y3 : y4;
    
    if (max_x1 < min_x2 || max_x2 < min_x1 || max_y1 < min_y2 || max_y2 < min_y1) {
        return false;
    }
    
    int32_t d1 = (int32_t)(x4 - x3) * (y1 - y3) - (int32_t)(y4 - y3) * (x1 - x3);
    int32_t d2 = (int32_t)(x4 - x3) * (y2 - y3) - (int32_t)(y4 - y3) * (x2 - x3);
    int32_t d3 = (int32_t)(x2 - x1) * (y3 - y1) - (int32_t)(y2 - y1) * (x3 - x1);
    int32_t d4 = (int32_t)(x2 - x1) * (y4 - y1) - (int32_t)(y2 - y1) * (x4 - x1);
    
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
        return true;
    }
    
    if (d1 == 0 || d2 == 0 || d3 == 0 || d4 == 0) {
        return true;
    }
    
    return false;
}

bool rect_intersects_rect(uint16_t x1, uint16_t y1, uint16_t w1, uint16_t h1,
                          uint16_t x2, uint16_t y2, uint16_t w2, uint16_t h2)
{
    return !(x1 + w1 < x2 || x2 + w2 < x1 || y1 + h1 < y2 || y2 + h2 < y1);
}

bool is_intersecting_coords(sprite* const self, coordinate_set_t coords)
{
    return rect_intersects_rect(self->x, self->y, self->width, self->height,
                                coords.x, coords.y, coords.w, coords.h);
}

bool colliding_with_normal_walls()
{
    sprite* character = spr_char_get();

    return is_intersecting_coords(character, wall_coordinates[0][0]) ||
           is_intersecting_coords(character, wall_coordinates[0][2]) ||
           is_intersecting_coords(character, wall_coordinates[2][0]) ||
           is_intersecting_coords(character, wall_coordinates[2][2]);
}

bool colliding_with_walls()
{
    if (colliding_with_normal_walls()) return true;

    sprite* character = spr_char_get();
    room* current_room = rooms[room_index];
    if (current_room->wall_collision_set_len == 0) return false;

    for (int i = 0; i < current_room->wall_collision_set_len; i++)
    {
        coordinate_set_t coords = current_room->wall_collision_sets[i];

        if (is_intersecting_coords(character, coords)) return true;
    }

    return false;
}

int colliding_with_room_transition()
{
    sprite* character = spr_char_get();
    room* current_room = rooms[room_index];

    for (int i = 0; i < current_room->trans_collision_set_len; i++)
    {
        coordinate_set_t coords = current_room->trans_collision_sets[i];

        if (is_intersecting_coords(character, coords)) return get_transition_index_by_coordinates(coords);
    }

    return -1;
}

bool rect_intersects_circle(uint16_t rect_x, uint16_t rect_y, 
                           uint16_t rect_w, uint16_t rect_h,
                           uint16_t circle_x, uint16_t circle_y, 
                           uint16_t circle_r)
{
    // Find the closest point on the rectangle to the circle center
    uint16_t closest_x = circle_x;
    uint16_t closest_y = circle_y;
    
    // Clamp circle center to rectangle bounds
    if (circle_x < rect_x) {
        closest_x = rect_x;
    } else if (circle_x > rect_x + rect_w) {
        closest_x = rect_x + rect_w;
    }
    
    if (circle_y < rect_y) {
        closest_y = rect_y;
    } else if (circle_y > rect_y + rect_h) {
        closest_y = rect_y + rect_h;
    }
    
    // Calculate distance between circle center and closest point
    int32_t dx = (int32_t)circle_x - (int32_t)closest_x;
    int32_t dy = (int32_t)circle_y - (int32_t)closest_y;
    int32_t distance_squared = dx * dx + dy * dy;
    int32_t radius_squared = (int32_t)circle_r * (int32_t)circle_r;
    
    return distance_squared <= radius_squared;
}

// Example usage with your coin defines:
bool sprite_intersects_coin(sprite* const self)
{
    return rect_intersects_circle(self->x, self->y, self->width, self->height,
                                  COIN_X, COIN_Y, COIN_R);
}

#define STATE_WALL_PERCENT 35.0f
#define STATE_HALL_PERCENT 0.0f
#define STATE_TRAN_PERCENT 42.0f
#define STATE_COIN_PERCENT 23.0f

int get_random_state(bool use_coins)
{
    float rand_percent = sys_rand(0, 10001) / 100.0f;
    
    if (rand_percent < STATE_WALL_PERCENT) 
        return ROOM_TILE_STATE_WALL;
    if (rand_percent < STATE_WALL_PERCENT + STATE_HALL_PERCENT) 
        return ROOM_TILE_STATE_HALL;
    if (rand_percent < STATE_WALL_PERCENT + STATE_HALL_PERCENT + STATE_TRAN_PERCENT) 
        return ROOM_TILE_STATE_TRAN;
    if (rand_percent < STATE_WALL_PERCENT + STATE_HALL_PERCENT + STATE_TRAN_PERCENT + STATE_COIN_PERCENT) 
        return use_coins ? ROOM_TILE_STATE_COIN : ROOM_TILE_STATE_TRAN;
    
    return ROOM_TILE_STATE_WALL;
}

void add_room(int transition_idx, room* transitioning_room)
{
    if (room_index == MAX_NUM_ROOMS-1)
        room_index = 0;
    else 
        room_index++;

    // Clean up of existing room.
    room* existing_room;
    if ((existing_room = rooms[room_index]) != NULL)
    {
        for (int i = 0; i < 4; i++)
        {
            if (existing_room->transition_points[i] != NULL)
                existing_room->transition_points[i]->transition_points[transition_complement_indexes[i]] = NULL;
        }

        rooms[room_index] = NULL;
        free(existing_room);
    }

    room* new_room = (room*)malloc(sizeof(room));
    room_structure_t room_structure;
    memcpy(room_structure, default_room_template, sizeof(room_structure_t));
    memset(new_room->wall_collision_sets, 0x0, sizeof(coordinate_set_t)*6);
    memset(new_room->trans_collision_sets, 0x0, sizeof(coordinate_set_t)*4);
    memset(new_room->transition_points, 0x0, sizeof(room*)*4);

    new_room->index = room_index;
    new_room->wall_collision_set_len = 0;
    new_room->trans_collision_set_len = 1;

    int complement_transition_idx = transition_complement_indexes[transition_idx];
    transitioning_room->transition_points[transition_idx] = new_room;
    new_room->transition_points[complement_transition_idx] = transitioning_room;
    coordinate_set_2x2_t transition_indices = transition_index_to_room_structure_indices[complement_transition_idx];
    new_room->trans_collision_sets[0] = trans_coordinates[transition_indices.x][transition_indices.y];

    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (default_room_template[row][col] == ROOM_TILE_STATE_TRAN && !(row == transition_indices.x && col == transition_indices.y))
            {
                int state = get_random_state(false);
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

                    if (state == ROOM_TILE_STATE_HALL)
                    {
                        new_room->wall_collision_set_len++;
                        new_room->wall_collision_sets[new_room->wall_collision_set_len-1] = hall_coordinates[row][col];

                        continue;
                    }
                }
            } else if (default_room_template[row][col] == ROOM_TILE_STATE_HALL)
            {
                int state = get_random_state(true);

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
    sprite* player = spr_char_get();
    room* current_room = rooms[room_index];
    int idx;

    if (current_room->structure[1][1] == ROOM_TILE_STATE_COIN)
    {
        if (sprite_intersects_coin(player))
        {
            current_room->structure[1][1] = ROOM_TILE_STATE_HALL;
            
            inc_infobox_coins();
            fill_circle(COIN_X, COIN_Y, COIN_R, 0x0000);
            render_sprite(player);
        }
    }

    if ((idx = colliding_with_room_transition()) >= 0)
    {
        player->oldx = player->x, player->oldy = player->y;

        render_sprite(player);
        room* transition_room = current_room->transition_points[idx];

        int complement_idx = transition_complement_indexes[idx];
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

        render_current_room();
    }
}