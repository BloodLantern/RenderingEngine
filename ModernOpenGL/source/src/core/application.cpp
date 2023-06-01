#include "core/application.hpp"

#include "core/object.hpp"
#include "core/input.hpp"
#include "core/debug/logger.hpp"
#include "resources/shader.hpp"
#include "resources/model.hpp"
#include "resources/texture.hpp"
#include "resources/resource_manager.hpp"
#include "ui/object_hierarchy.hpp"
#include "ui/object_inspector.hpp"
#include "low_renderer/light.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <numbers>

Application::Application()
{
    Logger::OpenDefaultFile();

    ObjectHierarchy* objectHierarchy = new ObjectHierarchy();
    mUIComponents.push_back(objectHierarchy);
    mUIComponents.push_back(new ObjectInspector(&objectHierarchy->selected));
}

Light* light;

bool Application::Initialize(const Vector2i windowSize, const char* const windowTitle)
{
    Logger::LogInfo("Initializing application with size [%d, %d] and title '%s'...", windowSize.x, windowSize.y, windowTitle);

    // Setup GLFW
    glfwSetErrorCallback(
        [](int error, const char* description)
        {
            Logger::LogError("GLFW error %d: %s", error, description);
        }
    );

    if (!glfwInit())
    {
        Logger::LogFatal("Failed to initialize GLFW");
        return false;
    }
    
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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable viewports
    io.Fonts->AddFontDefault();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    Logger::LogInfo("Application successfully initialized");

    Object* object = new Object("Viking room");
    object->mesh = new Mesh(
        ResourceManager::Load<Model>("assets/meshes/viking_room.obj"),
        ResourceManager::Load<Texture>("assets/textures/viking_room.jpg"),
        ResourceManager::Load<Shader>("source/shaders")
    );
    object->transform.rotation.x = -std::numbers::pi_v<float> / 2;
    mScene.root.AddChild(object);

    light = new Light(0, { 0.1f, 0.1f, 0.1f, 1.f }, { 0, 0, 1, 1.f }, 0);
    light->name = "Moon";
    light->mesh = new Mesh(
        ResourceManager::Load<Model>("assets/meshes/moon.obj"),
        ResourceManager::Load<Texture>("assets/textures/moon.png"),
        nullptr
    );
    mScene.root.AddChild(light);

    Input::Initialize(mWindow);

    glfwShowWindow(mWindow);

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

        Input::Update();

        if (!mScene.camera.fpsView && Input::keyboardKeyPress[inputs::KeyboardKey_Escape])
            break;

        mScene.Update(mDeltaTime);
        light->UpdateShader(*ResourceManager::Get<Shader>("source/shaders"));
        mScene.Draw();
        for (UIComponent* component : mUIComponents)
            component->Show(mScene);

        ShowDebugWindow();

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

void Application::ShowDebugWindow()
{
    static bool showInputs = false;

    ImGui::Begin("Debug");
    ImGui::Checkbox("Show inputs", &showInputs);
    ImGui::End();

    if (showInputs)
    {
        ImGui::Begin("Inputs");
        if (ImGui::TreeNode("Mouse"))
        {
            ImGui::Text("Position: %d, %d", Input::mousePosition.x, Input::mousePosition.y);
            ImGui::Text("Button down right: %d", Input::mouseDown[inputs::MouseButton_Right]);
            ImGui::Text("Button release right: %d", Input::mouseRelease[inputs::MouseButton_Right]);
            for (unsigned char i = 0; i < inputs::MouseButton_MaxCount; i++)
            {
                ImGui::Text("Button down %d: %d", i + 1, Input::mouseDown[i]);
                ImGui::Text("Button release %d: %d", i + 1, Input::mouseRelease[i]);
            }
            ImGui::Text("Wheel: %f, %f", Input::mouseWheel.x, Input::mouseWheel.y);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Keyboard"))
        {
            ImGui::Text("Key down B: %d", Input::keyboardKeyDown[inputs::KeyboardKey_B]);
            ImGui::Text("Key release B: %d", Input::keyboardKeyRelease[inputs::KeyboardKey_B]);
            ImGui::TreePop();
        }

        ImGui::Text("Controllers connected: %u", Input::controllerConnectedCount);
        for (unsigned char i = 0; i < Input::controllerConnectedCount; i++)
        {
            char nameBuffer[14];
            sprintf_s(nameBuffer, "Controller %u", i + 1);
            if (ImGui::TreeNode(nameBuffer))
            {
                ImGui::Text("Left stick axis: %f, %f",
                    Input::controllerStickAxis[i][inputs::Controller_StickLeft].x,
                    Input::controllerStickAxis[i][inputs::Controller_StickLeft].y);
                ImGui::Text("Right stick axis: %f, %f",
                    Input::controllerStickAxis[i][inputs::Controller_StickRight].x,
                    Input::controllerStickAxis[i][inputs::Controller_StickRight].y);

                ImGui::Text("Left trigger axis: %f", Input::controllerTriggerAxis[i][inputs::Controller_TriggerLeft]);
                ImGui::Text("Right trigger axis: %f", Input::controllerTriggerAxis[i][inputs::Controller_TriggerRight]);

                for (unsigned char j = 0; j < inputs::Controller_ButtonCount; j++)
                    ImGui::Text("Button down %d: %d", j + 1, Input::controllerButtonDown[i][j]);

                ImGui::Text("Directional pad direction: %u", Input::controllerDirectionalPad[i]);

                ImGui::TreePop();
            }
        }
        ImGui::End();
    }
}
