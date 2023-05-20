#include "core/application.hpp"

#include "core/debug/logger.hpp"
#include "resources/shader.hpp"

Application::Application()
{
    Logger::OpenDefaultFile();
}

bool Application::Initialize(const Vector2i windowSize, const char* const windowTitle)
{
    Logger::LogInfo("Initializing application with size { %d, %d } and title %s...", windowSize.x, windowSize.y, windowTitle);

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

    Logger::LogInfo("Application successfully initialized");

    Shader shader("source/shaders/vertex.glsl", "source/shaders/fragment.glsl");
    shader.Link();

    return true;
}

void Application::MainLoop()
{
    while (!glfwWindowShouldClose(mWindow))
    {
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
}

void Application::Shutdown()
{
    Logger::LogInfo("Shutting down application...");
	glfwDestroyWindow(mWindow);
    glfwTerminate();
    Logger::Stop();
}
