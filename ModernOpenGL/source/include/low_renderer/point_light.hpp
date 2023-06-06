#pragma once

#include "low_renderer/light.hpp"

class PointLight : public Light
{
public:    
    float constant = 1;
    float linear = 0.09f;
    float quadratic = 0.032f;

    PointLight() = default;
    PointLight(const Vector3& position, const Vector3& ambient, const Vector3& diffuse, const Vector3& specular)
        : Light(position, ambient, diffuse, specular) {}

    void UpdateShader(const Shader& shader) const override;
};
