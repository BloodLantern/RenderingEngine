#pragma once

#include <vector2.hpp>
#include <vector3.hpp>
#include <vector4.hpp>

struct Vertex
{
    Vector3 position;
    Vector2 uv;
    Vector3 normal;

    Vertex() = default;
    Vertex(const Vector3& position, const Vector2& uv, const Vector3& normal)
        : position(position), uv(uv), normal(normal) {}
};
