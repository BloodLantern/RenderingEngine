#pragma once

#include "low_renderer/light.hpp"

class DirectionalLight : public Light
{
public:
    Vector3 direction;

    DirectionalLight() = default;
    DirectionalLight(const Vector3& direction, const Vector3& ambient, const Vector3& diffuse, const Vector3& specular)
        : Light(0, ambient, diffuse, specular), direction(direction) {}

    void UpdateShader(const Shader& shader) const override;
};
