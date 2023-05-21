#include "low_renderer/mesh.hpp"

void Mesh::Draw()
{
    shader->Use();
    texture->Use();
    model->Draw();
}
