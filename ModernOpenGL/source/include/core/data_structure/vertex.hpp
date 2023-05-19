#pragma once

#include <vector2.hpp>
#include <vector3.hpp>
#include <vector4.hpp>

struct Vertex
{
    Vector3 Position;
    Vector3 Normal;
    Vector2 TextureUV;
    //Vector4 Color;

    Vertex() = default;
    Vertex(const Vector3& position, const Vector3& normal, const Vector2& textureUV)
        : Position(position), Normal(normal), TextureUV(textureUV) {}
};
