#pragma once

#include <vector2.hpp>
#include <vector3.hpp>
#include <vector4.hpp>

struct Vertex
{
    Vector3 Position;
    Vector2 UV;
    Vector3 Normal;

    Vertex() = default;
    Vertex(const Vector3& position, const Vector2& uv, const Vector3& normal)
        : Position(position), UV(uv), Normal(normal) {}
};
