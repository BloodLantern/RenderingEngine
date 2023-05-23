#pragma once

class Scene;

class UIComponent
{
public:
    virtual ~UIComponent() = default;

    virtual void Show(Scene& scene) = 0;
};
