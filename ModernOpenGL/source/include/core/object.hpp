#pragma once

#include "physics/transform.hpp"
#include "low_renderer/mesh.hpp"

class Object
{
public:
    std::string name;
    Transform transform;
    Mesh* mesh;

    Object(const std::string& name = "");
    Object(Mesh* const mesh = nullptr, const std::string& name = "");

    void Draw() const { if (mesh) mesh->Draw(); }

private:
};
