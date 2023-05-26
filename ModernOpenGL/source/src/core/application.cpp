#include "core/application.hpp"

#include "core/object.hpp"
#include "core/debug/logger.hpp"
#include "resources/shader.hpp"
#include "resources/model.hpp"
#include "resources/texture.hpp"
#include "resources/resource_manager.hpp"
#include "ui/object_hierarchy.hpp"
#include "ui/object_inspector.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

Application::Application()
{
    Logger::OpenDefaultFile();

    ObjectHierarchy* objectHierarchy = new ObjectHierarchy();
    mUIComponents.push_back(objectHierarchy);
    mUIComponents.push_back(new ObjectInspector(&objectHierarchy->selected));
}

bool Application::Initialize(const Vector2i windowSize, const char* const windowTitle)
{
    Logger::LogInfo("Initializing application with size [%d, %d] and title '%s'...", windowSize.x, windowSize.y, windowTitle);

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

    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable transparency
    //glEnable(GL_BLEND);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable viewports
    io.Fonts->AddFontDefault();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Logger::LogInfo("Application successfully initialized");

    Object* object = new Object("Viking room");
    object->mesh = new Mesh(
        ResourceManager::Load<Model>("assets\\meshes\\viking_room.obj"),
        ResourceManager::Load<Texture>("assets\\textures\\viking_room.jpg"),
        ResourceManager::Load<Shader>("source\\shaders")
    );
    mScene.root.AddChild(object);

    return true;
}

void Application::MainLoop()
{
    while (!glfwWindowShouldClose(mWindow))
    {
        const float time = (float) glfwGetTime();
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        mScene.Update(mDeltaTime);
        mScene.Draw();
        for (UIComponent* component : mUIComponents)
            component->Show(mScene);

        // Rendering
        ImGui::Render();
        int displayW, displayH;
        glfwGetFramebufferSize(mWindow, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLFWwindow* ctxBackup = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(ctxBackup);

        glfwSwapBuffers(mWindow);
        mDeltaTime = ((float) glfwGetTime()) - time;
    }
}

void Application::Shutdown()
{
    Logger::LogInfo("Shutting down application...");

    for (UIComponent* component : mUIComponents)
        delete component;
    mScene.DeleteAllObjects();
    ResourceManager::UnloadAll();

    // Shutdown ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    

	glfwDestroyWindow(mWindow);
    glfwTerminate();

    Logger::Stop();
}
