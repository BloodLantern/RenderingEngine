#include "low_renderer/point_light.hpp"

void PointLight::UpdateShader(const Shader& shader) const
{
    std::string id = std::to_string(index);
    shader.SetUniform("pointLights[" + id + "].position", (Vector3) (transform.GetGlobal() * Vector4(1)));

    shader.SetUniform("pointLights[" + id + "].constant", constant);
    shader.SetUniform("pointLights[" + id + "].linear", linear);
    shader.SetUniform("pointLights[" + id + "].quadratic", quadratic);

    shader.SetUniform("pointLights[" + id + "].color.ambient", ambient);
    shader.SetUniform("pointLights[" + id + "].color.diffuse", diffuse);
    shader.SetUniform("pointLights[" + id + "].color.specular", specular);
}
