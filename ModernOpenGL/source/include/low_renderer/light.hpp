#pragma once

#include "core/object.hpp"

#include <vector4.hpp>

class Light : public Object
{
public:
    Vector4 ambient, diffuse, specular;

    Light() = default;
    Light(const Vector3& position, const Vector4& ambient, const Vector4& diffuse, const Vector4& specular)
        : Object(position), ambient(ambient), diffuse(diffuse), specular(specular) {}
        
    void Update(const float deltaTime) override;
    void UpdateShader(const Shader& shader) const;

private:
};
