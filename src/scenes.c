#include <scenes.h>

SCENE_GET_DEF(current); // psuedo scene

void init_scenes()
{
    SCENE_F(menu, init)();
    SCENE_F(game, init)();
}

void step_scenes(const input_status* const input)
{
    if (!s_current_scene) return;

    s_current_scene->step(input);
}

void change_scene(scene* scene)
{
    if (s_current_scene != scene)
    {
        s_current_scene = scene;
        s_current_scene->on_change();
    }
}