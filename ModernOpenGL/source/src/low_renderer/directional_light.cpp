#include "low_renderer/directional_light.hpp"

void DirectionalLight::UpdateShader(const Shader& shader) const
{
    std::string id = std::to_string(index);
    shader.SetUniform("directionalLights[" + id + "].direction", direction);
    shader.SetUniform("directionalLights[" + id + "].color.ambient", ambient);
    shader.SetUniform("directionalLights[" + id + "].color.diffuse", diffuse);
    shader.SetUniform("directionalLights[" + id + "].color.specular", specular);
}
