/**
 * @file: scenes/game_scene.h
 * @desc: Game scene interface - main gameplay scene
 * @auth: James Farrelly (C24402114)
 */

#ifndef SCENES_GAME_SCENE_H_
#define SCENES_GAME_SCENE_H_

#include <scenes/scene.h>

SCENE_GET_DEC(game);
void SCENE_F(game, init)(void);

#endif // !SCENES_GAME_SCENE_H_