#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <stdint.h>

#include <io.h>

typedef struct character 
{
    uint8_t width, height;
    const uint16_t* img_data;
    uint8_t x, y, oldx, oldy;
    uint8_t horientation, vorientation;
} character;

void init_character(void);
void step_character(const input_status* const input);
void render_character(void);

character* get_character(void);

#endif