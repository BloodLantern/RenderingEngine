#include "low_renderer/mesh.hpp"

void Mesh::Draw(const Matrix4x4& trs, const Matrix4x4& viewProjection) const
{
    if (shader)
        shader->Use();
    if (texture)
        texture->Use();
    if (shader)
    {
        const Matrix4x4&& newTrs = Matrix4x4::Translation3D(position) * trs;
        shader->SetUniform("mvp", viewProjection * newTrs);
        shader->SetUniform("model", newTrs);
    }
    if (model)
        model->Draw();
}
