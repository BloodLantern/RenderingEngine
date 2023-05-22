#pragma once

#include "core/object.hpp"

class Scene
{
public:
    Object root = std::string("Scene");

    void Update(const float deltaTime) { root.Update(deltaTime); }
    void Draw() { root.Draw(); }
};
