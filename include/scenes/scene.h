/**
 * @file: scenes/scene.h
 * @desc: Base scene structure and macros
 * @auth: James Farrelly (C24402114)
 */

#ifndef SCENES_SCENE_H_
#define SCENES_SCENE_H_

#include <io.h>

/**
 * Generate scene function name
 * @param x Scene name identifier
 * @param f Function name component
 */
#define SCENE_F(x, f) f##_##x##_scene

/**
 * Get scene instance by name
 * @param x Scene name identifier
 */
#define SCENE(x) SCENE_F(x, get)()

/**
 * Declare scene getter function
 * @param x Scene name identifier
 */
#define SCENE_GET_DEC(x) scene* SCENE_F(x, get)(void)

/**
 * Define scene getter function with static scene instance
 * @param x Scene name identifier
 */
#define SCENE_GET_DEF(x) static scene* s_##x##_scene; SCENE_GET_DEC(x) { return s_##x##_scene; }

/**
 * Scene structure containing callback functions
 */
typedef struct scene 
{
    void (*on_change)();                            /* Called when scene becomes active */
    void (*step)(const input_status* const input); /* Called each frame to update scene */
} scene;

#endif // !SCENES_SCENE_H_