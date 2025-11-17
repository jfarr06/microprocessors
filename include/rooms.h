#ifndef ROOMS_H_
#define ROOMS_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t room_structure_t[3][3];

typedef struct coordinate_set
{
    uint8_t x, y, w, h;

    const uint16_t* img_data;
} coordinate_set_t;

typedef struct coordinate_set_2x2
{
    uint8_t x, y;
} coordinate_set_2x2_t;

typedef struct room {
    uint8_t index;

    room_structure_t structure;
    struct room* transition_points[4]; // 0 = top, 1 = left, 2 = right, 3 = bottom
                                       // Complement: 0 = 3, 1 = 2, 2 = 1, 3 = 0
    uint8_t wall_collision_set_len;
    coordinate_set_t wall_collision_sets[3]; // Only 3 can be present at max

    uint8_t trans_collision_set_len;
    coordinate_set_t trans_collision_sets[4];
} room;

#define MAX_NUM_ROOMS 5

#define ROOM_TILE_STATE_WALL 0x0
#define ROOM_TILE_STATE_HALL 0x1
#define ROOM_TILE_STATE_TRAN 0x2
#define ROOM_TILE_STATE_COIN 0x3

void init_rooms(void);
void step_rooms(void);

bool colliding_with_walls(void);

#endif // !ROOMS_H_