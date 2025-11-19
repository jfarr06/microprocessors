/**
 * @file: scenes/menu_scene.h
 * @desc: Menu scene interface - home menu and mode selection
 * @auth: James Farrelly (C24402114)
 */

#ifndef SCENES_MENU_SCENE_H_
#define SCENES_MENU_SCENE_H_

#include <scenes/scene.h>

SCENE_GET_DEC(menu);
void SCENE_F(menu, init)(void);

#endif // !SCENES_MENU_SCENE_H_