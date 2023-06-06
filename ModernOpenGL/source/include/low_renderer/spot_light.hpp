#pragma once

#include "low_renderer/light.hpp"

class SpotLight : public Light
{
public:
    Vector3 direction;

    float constant = 1;
    float linear = 0.09f;
    float quadratic = 0.032f;
    
    float cutOff;
    float outerCutOff;

    SpotLight() = default;
    SpotLight(const Vector3& position, const Vector3& direction, const float cutOff, const float outerCutOff,
        const Vector3& ambient, const Vector3& diffuse, const Vector3& specular)
        : Light(position, ambient, diffuse, specular), direction(direction), cutOff(cutOff), outerCutOff(outerCutOff) {}

    void UpdateShader(const Shader& shader) const override;
};
