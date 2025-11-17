#ifndef SCENES_SCENE_H_
#define SCENES_SCENE_H_

#include <io.h>

#define SCENE_F(x, f) f##_##x##_scene

#define SCENE(x) SCENE_F(x, get)()

#define SCENE_GET_DEC(x) scene* SCENE_F(x, get)(void)
#define SCENE_GET_DEF(x) static scene* s_##x##_scene; SCENE_GET_DEC(x) { return s_##x##_scene; }

typedef struct scene 
{
    void (*on_change)();
    void (*step)(const input_status* const input);
} scene;

#endif // !SCENES_SCENE_H_