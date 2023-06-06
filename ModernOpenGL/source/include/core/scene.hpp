#pragma once

#include "core/object.hpp"
#include "low_renderer/camera.hpp"
#include "low_renderer/directional_light.hpp"
#include "low_renderer/point_light.hpp"
#include "low_renderer/spot_light.hpp"

#define MAX_DIRECTIONAL_LIGHT_COUNT 4
#define MAX_POINT_LIGHT_COUNT 10
#define MAX_SPOT_LIGHT_COUNT 10
#define LIGHT_COUNT (MAX_DIRECTIONAL_LIGHT_COUNT + MAX_POINT_LIGHT_COUNT + MAX_SPOT_LIGHT_COUNT)

class Scene
{
public:
    Object root = std::string("Scene");
    Object lightsRoot = std::string("Lights");

    Camera camera;

    DirectionalLight* directionalLights[MAX_DIRECTIONAL_LIGHT_COUNT];
    PointLight* pointLights[MAX_POINT_LIGHT_COUNT];
    SpotLight* spotLights[MAX_SPOT_LIGHT_COUNT];
    Light* lights[LIGHT_COUNT];

    ~Scene();

    void Update(const float deltaTime);
    void Draw();

    void ForEachObject(std::function<void(Object*)> func) { root.ForEachChild(func); }
    void DeleteAllObjects() { root.DeleteChildren(); }
};
