#ifndef SCENES_H_
#define SCENES_H_

#include <io.h>
#include <scenes/scene.h>

void init_scenes(void);
void step_scenes(const input_status* const input);

void change_scene(scene* scene);

SCENE_GET_DEC(current);

#include <scenes/menu_scene.h>
#include <scenes/game_scene.h>

#endif