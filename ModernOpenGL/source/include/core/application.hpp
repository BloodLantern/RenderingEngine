#pragma once

#include <vector2i.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Application
{
public:
    Application();

    bool Initialize(const Vector2i windowSize, const char* const windowTitle);
    void MainLoop();
    void Shutdown();

private:
    bool mInitialized;
    GLFWwindow* mWindow;
};
