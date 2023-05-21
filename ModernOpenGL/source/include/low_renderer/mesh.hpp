#pragma once

#include "resources/model.hpp"
#include "resources/texture.hpp"
#include "resources/shader.hpp"

struct Mesh
{
    Model* model;
    Texture* texture;
    Shader* shader;

    Mesh(Model* const model, Texture* const texture, Shader* const shader)
        : model(model), texture(texture), shader(shader) {}

    void Draw();
};
