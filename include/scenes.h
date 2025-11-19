/**
 * @file: scenes.h
 * @desc: Scene management system interface
 * @auth: James Farrelly (C24402114)
 */

#ifndef SCENES_H_
#define SCENES_H_

#include <io.h>
#include <scenes/scene.h>

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
 * Get the current active scene
 */
SCENE_GET_DEC(current);

#include <scenes/menu_scene.h>
#include <scenes/game_scene.h>

#endif