#pragma once

#include "ui/ui_component.hpp"

class Object;

class ObjectInspector : public UIComponent
{
public:
    ObjectInspector(Object* const* const object)
        : mObject(object) {}

    virtual void Show(Scene& scene) override;

private:
    Object* const* const mObject = nullptr;
};
