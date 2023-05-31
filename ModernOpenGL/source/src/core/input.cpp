#include "core/input.hpp"

Vector2i Input::mousePosition;
bool Input::mouseDown[inputs::MouseButton_MaxCount];
bool Input::mousePress[inputs::MouseButton_MaxCount];
bool Input::mouseRelease[inputs::MouseButton_MaxCount];
Vector2 Input::mouseWheel;

bool Input::keyboardKeyDown[inputs::KeyboardKey_MaxCount];
bool Input::keyboardKeyPress[inputs::KeyboardKey_MaxCount];
bool Input::keyboardKeyRelease[inputs::KeyboardKey_MaxCount];

unsigned char Input::controllerConnectedCount = 0;
bool Input::controllerConnected[inputs::Controller_MaxCount];
Vector2 Input::controllerStickAxis[inputs::Controller_MaxCount][inputs::Controller_StickCount];
float Input::controllerTriggerAxis[inputs::Controller_MaxCount][inputs::Controller_TriggerCount];
Vector2 Input::controllerStickAxisDeadzones[inputs::Controller_MaxCount][inputs::Controller_StickCount];
float Input::controllerTriggerAxisDeadzones[inputs::Controller_MaxCount][inputs::Controller_TriggerCount];
bool Input::controllerButtonDown[inputs::Controller_MaxCount][inputs::Controller_ButtonCount];
bool Input::controllerButtonPress[inputs::Controller_MaxCount][inputs::Controller_ButtonCount];
bool Input::controllerButtonRelease[inputs::Controller_MaxCount][inputs::Controller_ButtonCount];
unsigned char Input::controllerDirectionalPad[inputs::Controller_MaxCount];

GLFWwindow* Input::mWindow = nullptr;

bool newMouseDown[inputs::MouseButton_MaxCount];
bool newKeyboardKeyDown[inputs::KeyboardKey_MaxCount];
Vector2 newMouseWheel;

#define CONTROLLER_DEADZONE_DEFAULT 0.1f

void MousePositionCallback(GLFWwindow*, double x, double y)
{
    Input::mousePosition = Vector2i((int) x, (int) y);
}

void MouseButtonCallback(GLFWwindow*, int button, int action, int)
{
    newMouseDown[button] = action != GLFW_RELEASE;
}

void MouseScrollCallback(GLFWwindow*, double xoffset, double yoffset)
{
    newMouseWheel = Vector2((float) xoffset, (float) yoffset);
}

void KeyCallback(GLFWwindow*, int key, int, int action, int)
{
    // Do not use index -1 in arrays
    if (key == GLFW_KEY_UNKNOWN)
        return;

    newKeyboardKeyDown[key] = action != GLFW_RELEASE;
}

void JoystickCallback(int jid, int event)
{
    if (event == GLFW_CONNECTED)
    {
        Input::controllerConnected[jid] = true;
        Input::controllerConnectedCount++;
    }
    else if (event == GLFW_DISCONNECTED)
    {
        Input::controllerConnected[jid] = false;
        Input::controllerConnectedCount--;
    }
}

void Input::SetCursorHidden(const bool cursorHidden)
{
    glfwSetInputMode(mWindow, GLFW_CURSOR, cursorHidden ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Input::Initialize(GLFWwindow *const window)
{
    mWindow = window;

    glfwSetCursorPosCallback(window, MousePositionCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, MouseScrollCallback);

    glfwSetKeyCallback(window, KeyCallback);

    glfwSetJoystickCallback(JoystickCallback);

    for (unsigned int i = 0; i < inputs::MouseButton_MaxCount; i++)
    {
        mouseDown[i] = false;
        newMouseDown[i] = false;
    }

    for (unsigned int i = 0; i < inputs::KeyboardKey_MaxCount; i++)
    {
        keyboardKeyDown[i] = false;
        newKeyboardKeyDown[i] = false;
    }

    // Count connected controllers
    for (unsigned int i = 0; i < inputs::Controller_MaxCount; i++)
    {
        const bool present = glfwJoystickPresent(i);
        if (present)
            controllerConnectedCount++;
        controllerConnected[i] = present;

        for (unsigned int j = 0; j < inputs::Controller_StickCount; j++)
            controllerStickAxisDeadzones[i][j] = CONTROLLER_DEADZONE_DEFAULT;

        for (unsigned int j = 0; j < inputs::Controller_TriggerCount; j++)
            controllerTriggerAxisDeadzones[i][j] = CONTROLLER_DEADZONE_DEFAULT;
    }
}

void Input::Update()
{
    for (unsigned int i = inputs::MouseButton_First; i < inputs::MouseButton_MaxCount; i++)
    {
        mouseRelease[i] = mouseDown[i] && !newMouseDown[i];
        mousePress[i] = !mouseDown[i] && newMouseDown[i];
        mouseDown[i] = newMouseDown[i];
    }
    Input::mouseWheel = newMouseWheel;
    newMouseWheel = 0;

    for (unsigned int i = 0; i < inputs::KeyboardKey_MaxCount; i++)
    {
        keyboardKeyRelease[i] = keyboardKeyDown[i] && !newKeyboardKeyDown[i];
        keyboardKeyPress[i] = !keyboardKeyDown[i] && newKeyboardKeyDown[i];
        keyboardKeyDown[i] = newKeyboardKeyDown[i];
    }

    for (unsigned int i = 0; i < inputs::Controller_MaxCount; i++)
        if (controllerConnected[i])
        {
            {
                // Stick and trigger axis
                int count;
                const float* newStickAxis = glfwGetJoystickAxes(i, &count);
                // In case the controller was disconnected and the events weren't polled yet
                if (count == 0)
                    continue;

                controllerStickAxis[i][0] = Vector2(newStickAxis[0], -newStickAxis[1]);
                controllerStickAxis[i][1] = Vector2(newStickAxis[2], -newStickAxis[5]);

                // Stick deadzones
                for (unsigned int j = 0; j < 2; j++)
                    for (unsigned int k = 0; k < 2; k++)
                        if (std::abs(controllerStickAxis[i][j][k]) < controllerStickAxisDeadzones[i][j][k])
                            controllerStickAxis[i][j][k] = 0;

                controllerTriggerAxis[i][0] = newStickAxis[3];
                controllerTriggerAxis[i][1] = newStickAxis[4];
                
                // Trigger deadzones
                for (unsigned int j = 0; j < 2; j++)
                    if (controllerTriggerAxis[i][j] < controllerTriggerAxisDeadzones[i][j])
                        controllerTriggerAxis[i][j] = 0;
            }

            {
                // Button states
                int count;
                const unsigned char* newButtonStates = glfwGetJoystickButtons(i, &count);
                // In case the controller was disconnected and the events weren't polled yet
                if (count == 0)
                    continue;

                for (unsigned int j = 0; j < inputs::Controller_ButtonCount; j++)
                {
                    controllerButtonRelease[i][j] = controllerButtonDown[i] && !newButtonStates[j];
                    controllerButtonPress[i][j] = !controllerButtonDown[i] && newButtonStates[j];
                    controllerButtonDown[i][j] = newButtonStates[j];
                }
            }

            {
                // Directional pad states
                int count;
                const unsigned char* newDirectionalPad = glfwGetJoystickHats(i, &count);
                // In case the controller was disconnected and the events weren't polled yet
                if (count == 0)
                    continue;

                controllerDirectionalPad[i] = newDirectionalPad[0];
            }
        }
}
