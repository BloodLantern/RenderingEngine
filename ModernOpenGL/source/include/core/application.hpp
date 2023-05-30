#pragma once

#include <vector2i.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "core/scene.hpp"
#include "ui/ui_component.hpp"

class Application
{
public:
    Application();

    bool Initialize(const Vector2i windowSize, const char* const windowTitle);
    void MainLoop();
    void Shutdown();

    void ShowDebugWindow();
    
private:
    bool mInitialized = false;
    GLFWwindow* mWindow = nullptr;

    Scene mScene;
    float mDeltaTime = 0;

    std::vector<UIComponent*> mUIComponents;
};
