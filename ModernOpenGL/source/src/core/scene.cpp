#include "core/scene.hpp"

#include <ImGui/imgui.h>

Scene::~Scene()
{
    for (size_t i = 0; i < MAX_DIRECTIONAL_LIGHT_COUNT; i++)
        delete directionalLights[i];
    for (size_t i = 0; i < MAX_POINT_LIGHT_COUNT; i++)
        delete pointLights[i];
    for (size_t i = 0; i < MAX_SPOT_LIGHT_COUNT; i++)
        delete spotLights[i];
}

void Scene::Update(const float deltaTime)
{
    size_t index = 0;
    for (size_t i = 0; i < MAX_DIRECTIONAL_LIGHT_COUNT; i++, index++)
        lights[index] = directionalLights[i];
    for (unsigned int i = 0; i < MAX_POINT_LIGHT_COUNT; i++, index++)
        if (pointLights[i])
        {
            pointLights[i]->index = i;
            lights[index] = pointLights[i];
        }
    for (size_t i = 0; i < MAX_SPOT_LIGHT_COUNT; i++, index++)
        lights[index] = spotLights[i];

    root.Update(deltaTime);
    camera.Update(deltaTime);
    lightsRoot.Update(deltaTime);
}

void Scene::Draw()
{
    root.Draw(camera.GetViewProjectionMatrix(), camera, lights);
    lightsRoot.Draw(camera.GetViewProjectionMatrix(), camera, nullptr);
}
