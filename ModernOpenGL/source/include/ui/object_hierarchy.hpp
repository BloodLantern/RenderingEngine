#pragma once

#include "ui/ui_component.hpp"

class Object;

class ObjectHierarchy : public UIComponent
{
public:
    Object* selected;

    virtual void Show(Scene& scene) override;

private:
    void ShowObject(Object* const object);
};
