#include "low_renderer/spot_light.hpp"

void SpotLight::UpdateShader(const Shader& shader) const
{
    std::string id = std::to_string(index);
    shader.SetUniform("spotLights[" + id + "].position", (Vector3) (transform.GetGlobal() * Vector4(1)));
    shader.SetUniform("spotLights[" + id + "].direction", direction);
    
    shader.SetUniform("spotLights[" + id + "].cutOff", cutOff);
    shader.SetUniform("spotLights[" + id + "].outerCutOff", outerCutOff);

    shader.SetUniform("spotLights[" + id + "].constant", constant);
    shader.SetUniform("spotLights[" + id + "].linear", linear);
    shader.SetUniform("spotLights[" + id + "].quadratic", quadratic);
    
    shader.SetUniform("spotLights[" + id + "].color.ambient", ambient);
    shader.SetUniform("spotLights[" + id + "].color.diffuse", diffuse);
    shader.SetUniform("spotLights[" + id + "].color.specular", specular);
}
