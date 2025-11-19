/**
 * @file: character.h
 * @desc: Character system interface - sprite animation and movement
 * @auth: James Farrelly (C24402114)
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <stdint.h>

#include <io.h>

/**
 * Character structure representing the player sprite
 */
typedef struct character 
{
    uint8_t width, height;          /* Sprite dimensions in pixels */
    const uint16_t* img_data;       /* Pointer to sprite image data */
    uint8_t x, y, oldx, oldy;       /* Current and previous position coordinates */
    uint8_t horientation, vorientation; /* Horizontal and vertical orientation flags */
} character;

/**
 * Initialize the character system and set initial position
 */
void init_character(void);

/**
 * Update character state based on input
 * @param input Pointer to input status containing button states
 */
void step_character(const input_status* const input);

/**
 * Render the character sprite to the display
 */
void render_character(void);

/**
 * Get pointer to the global character instance
 * @return Pointer to the character structure
 */
character* get_character(void);

#endif