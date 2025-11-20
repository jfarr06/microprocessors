/**
 * @file: scenes.h
 * @desc: Scene management system interface
 * @auth: James Farrelly (C24402114)
 */

#ifndef SCENES_H_
#define SCENES_H_

#include <stdint.h>
#include <stdbool.h>

#include <io.h>
#include <font5x7.h>
#include <scenes/scene.h>

#define LINE(x) ((x) * FONT_HEIGHT)             // Gets the line pixels for x amount of characters
#define LINE_BIG(x) (LINE(x) * 2)               // Gets the line pixels for x amount of 2x scaled characters
#define CHARS(x) ((x) * FONT_WIDTH * 2)+((x)*2) // Gets the width pixels for x amount of 2x scaled characters

/**
 * Initialize the scene management system
 */
void init_scenes(void);

/**
 * Update the current active scene
 * @param input Pointer to input status
 */
void step_scenes(const input_status* const input);

/**
 * Change to a different scene
 * @param scene Pointer to the new scene to activate
 */
void change_scene(scene* scene);

/**
 * Steps options scene.
 *
 * @param input The input object
 * @param num_options The max number of options
 * @param selected_option The current/changed option
 * @returns True if the options should be rerendered, false otherwise.
 */
bool step_options(const input_status* const input, uint8_t num_options, uint8_t* const selected_option);

/**
 * Renders options
 *
 * @param options The option texts
 * @param option_sizes The size of each option text
 * @param option_colours The colour of each option text
 * @param num_options The number of options
 * @param selected_option The currently selected option
 * @param xOffset The xOffset in pixels.
 * @param yOffset The yOffset in pixels.
 */
void render_options(
    const char* const options[], 
    const uint8_t option_sizes[],
    const uint16_t option_colours[],
    const uint8_t num_options, 
    const uint8_t selected_option, 
    const uint8_t xOffset, 
    const uint8_t yOffset
);

/**
 * Get the current active scene
 */
SCENE_GET_DEC(current);

#include <scenes/menu_scene.h>
#include <scenes/game_scene.h>

#endif
