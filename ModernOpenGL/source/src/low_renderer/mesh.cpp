#include "low_renderer/mesh.hpp"

#include "core/scene.hpp"

void Mesh::Draw(const Matrix4x4& trs, const Matrix4x4& viewProjection, const Camera& camera, const Light* const* const lights) const
{
    if (!shader)
        return;

    shader->Use();

    int directionalLightCount = 0, pointLightCount = 0, spotLightCount = 0;
    if (lights)
        for (size_t i = 0; i < LIGHT_COUNT; i++)
            if (lights[i])
            {
                lights[i]->UpdateShader(*shader);
                if (dynamic_cast<const DirectionalLight* const>(lights[i]))
                    directionalLightCount++;
                else if (dynamic_cast<const PointLight* const>(lights[i]))
                    pointLightCount++;
                else if (dynamic_cast<const SpotLight* const>(lights[i]))
                    spotLightCount++;
            }

    // Vertex shader
    shader->SetUniform("mvp", viewProjection * trs);
    shader->SetUniform("model", trs);

    // Fragment shader
    shader->SetUniform("cameraPosition", camera.transform.position);

    shader->SetUniform("directionalLightCount", directionalLightCount);
    shader->SetUniform("pointLightCount", pointLightCount);
    shader->SetUniform("spotLightCount", spotLightCount);
    
    shader->SetUniform("material.shininess", 32.f);

    if (texture)
        texture->Use();
    if (model)
        model->Draw();
}
