#include "low_renderer/mesh.hpp"

void Mesh::Draw(const Matrix4x4& modelViewProjection) const
{
    shader->Use();
    texture->Use();
    shader->SetUniform("mvp", modelViewProjection);
    model->Draw();
}
