#pragma once

#include "core/object.hpp"
#include "low_renderer/camera.hpp"

class Scene
{
public:
    Object root = std::string("Scene");
    Camera camera;

    void Update(const float deltaTime);
    void Draw();

    void ForEachObject(std::function<void(Object*)> func) { root.ForEachChild(func); }
    void DeleteAllObjects() { root.DeleteChildren(); }
};
