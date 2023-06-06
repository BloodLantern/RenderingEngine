#pragma once

#include "core/object.hpp"
#include "resources/shader.hpp"

#include <vector4.hpp>

class Light : public Object
{
public:
    unsigned int index;
    Vector3 ambient, diffuse, specular;

    Light() = default;
    Light(const Vector3& position, const Vector4& ambient, const Vector4& diffuse, const Vector4& specular)
        : Object(position), ambient(ambient), diffuse(diffuse), specular(specular) {}
        
    virtual void UpdateShader(const Shader& shader) const = 0;

private:
};
