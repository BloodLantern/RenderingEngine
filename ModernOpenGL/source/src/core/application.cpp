#include "core/application.hpp"

#include "core/debug/logger.hpp"
#include "resources/shader.hpp"
#include "resources/model.hpp"
#include "resources/resource_manager.hpp"

Application::Application()
{
    Logger::OpenDefaultFile();
}

Shader* shader;
Model* model;

bool Application::Initialize(const Vector2i windowSize, const char* const windowTitle)
{
    Logger::LogInfo("Initializing application with size [%d, %d] and title %s...", windowSize.x, windowSize.y, windowTitle);

    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // Create window
    mWindow = glfwCreateWindow(windowSize.x, windowSize.y, windowTitle, NULL, NULL);
    if (mWindow == NULL)
    {
        Logger::LogFatal("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(1); // Enable vsync

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        Logger::LogFatal("Failed to initialize GLAD");
        return false;
    }

    // Set window resize callback and viewport size
    glfwSetFramebufferSizeCallback(mWindow, [] (GLFWwindow*, int width, int height) { glViewport(0, 0, width, height); });
    glViewport(0, 0, windowSize.x, windowSize.y);

	glfwShowWindow(mWindow);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable transparency
    //glEnable(GL_BLEND);

    Logger::LogInfo("Application successfully initialized");

    shader = ResourceManager::Load<Shader>("source\\shaders");
    model = ResourceManager::Load<Model>("assets\\meshes\\viking_room.obj");

    return true;
}

void Application::MainLoop()
{
    while (!glfwWindowShouldClose(mWindow))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->Use();
        model->Draw();

        glfwSwapBuffers(mWindow);
    }
}

void Application::Shutdown()
{
    Logger::LogInfo("Shutting down application...");

    ResourceManager::UnloadAll();

	glfwDestroyWindow(mWindow);
    glfwTerminate();

    Logger::Stop();
}
