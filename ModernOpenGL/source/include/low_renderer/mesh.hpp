#pragma once

#include "resources/model.hpp"
#include "resources/texture.hpp"
#include "resources/shader.hpp"

class Mesh
{
public:
    Model* model;
    Texture* texture;
    Shader* shader;
    Vector3 position;

    Mesh(Model* const model, Texture* const texture, Shader* const shader)
        : model(model), texture(texture), shader(shader) {}

    void Draw(const Matrix4x4& trs, const Matrix4x4& viewProjection) const;
};
