#pragma once

#include "ui/ui_component.hpp"

class Object;

class ObjectHierarchy : public UIComponent
{
public:
    virtual void Show(Scene& scene) override;

private:
    void ShowObject(const Object* const object);
};
