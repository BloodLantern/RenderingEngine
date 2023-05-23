#pragma once

#include "ui/ui_component.hpp"

class ObjectInspector : public UIComponent
{
public:
    virtual void Show(Scene& scene) override;
};
