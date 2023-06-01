#include "low_renderer/light.hpp"

void Light::Update(const float deltaTime)
{
    Object::Update(deltaTime);
}

void Light::UpdateShader(const Shader &shader) const
{
    shader.SetUniform("lightPosition", transform.position);
    shader.SetUniform("ambientColor", ambient);
    shader.SetUniform("diffuseColor", diffuse);
    shader.SetUniform("specularColor", specular);
}
