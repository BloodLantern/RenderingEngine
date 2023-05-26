#include "core/scene.hpp"

#include <ImGui/imgui.h>

void Scene::Update(const float deltaTime)
{
    root.Update(deltaTime);
    camera.Update(deltaTime);
}

void Scene::Draw()
{
    root.Draw(camera.GetViewProjectionMatrix());
}
