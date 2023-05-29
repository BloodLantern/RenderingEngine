#include "core/input.hpp"

Vector2i Input::mousePosition;
bool Input::mouseDown[inputs::MouseButton_MaxCount];
bool Input::mouseRelease[inputs::MouseButton_MaxCount];
Vector2 Input::mouseWheel;

bool Input::keyboardKeyDown[inputs::KeyboardKey_MaxCount];
bool Input::keyboardKeyRelease[inputs::KeyboardKey_MaxCount];

unsigned char Input::controllerConnectedCount = 0;
bool Input::controllerConnected[inputs::Controller_MaxCount];
Vector2 Input::controllerStickAxis[inputs::Controller_MaxCount][inputs::Controller_StickCount];
float Input::controllerTriggerAxis[inputs::Controller_MaxCount][inputs::Controller_TriggerCount];
bool Input::controllerButton[inputs::Controller_MaxCount][inputs::Controller_ButtonCount];
unsigned char Input::controllerDirectionalPad[inputs::Controller_MaxCount];

void MousePositionCallback(GLFWwindow*, double x, double y)
{
    Input::mousePosition = Vector2i((int) x, (int) y);
}

bool mouseReleasedLastFrame[inputs::MouseButton_MaxCount];

void MouseButtonCallback(GLFWwindow*, int button, int action, int)
{
    if (action == GLFW_PRESS)
        Input::mouseDown[button] = true;
    else if (action == GLFW_RELEASE)
    {
        Input::mouseDown[button] = false;
        Input::mouseRelease[button] = true;
        mouseReleasedLastFrame[button] = true;
    }
}

void MouseScrollCallback(GLFWwindow*, double xoffset, double yoffset)
{
    Input::mouseWheel = Vector2((float) xoffset, (float) yoffset);
}

bool keyReleasedLastFrame[inputs::KeyboardKey_MaxCount];

void KeyCallback(GLFWwindow*, int key, int, int action, int)
{
    // Do not use index -1 in arrays
    if (key == GLFW_KEY_UNKNOWN)
        return;

    if (action == GLFW_PRESS)
        Input::keyboardKeyDown[key] = true;
    else if (action == GLFW_RELEASE)
    {
        Input::keyboardKeyDown[key] = false;
        Input::keyboardKeyRelease[key] = true;
        keyReleasedLastFrame[key] = true;
    }
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

void Input::Initialize(GLFWwindow* const window)
{
    glfwSetCursorPosCallback(window, MousePositionCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, MouseScrollCallback);

    glfwSetKeyCallback(window, KeyCallback);

    glfwSetJoystickCallback(JoystickCallback);

    for (unsigned int i = 0; i < inputs::MouseButton_MaxCount; i++)
    {
        mouseDown[i] = false;
        mouseRelease[i] = false;
        mouseReleasedLastFrame[i] = false;
    }

    for (unsigned int i = 0; i < inputs::KeyboardKey_MaxCount; i++)
    {
        keyboardKeyDown[i] = false;
        keyboardKeyRelease[i] = false;
    }

    // Count connected controllers
    for (unsigned int i = 0; i < inputs::Controller_MaxCount; i++)
    {
        const bool present = glfwJoystickPresent(i);
        if (present)
            controllerConnectedCount++;
        controllerConnected[i] = present;
    }
}

void Input::Update()
{
    for (unsigned int i = 0; i < inputs::MouseButton_MaxCount; i++)
        if (mouseRelease[i])
        {
            if (!mouseReleasedLastFrame[i])
                mouseRelease[i] = false;
            else
                mouseReleasedLastFrame[i] = false;
        }
    mouseWheel = 0;

    for (unsigned int i = 0; i < inputs::KeyboardKey_MaxCount; i++)
        if (keyboardKeyRelease[i])
        {
            if (!keyReleasedLastFrame[i])
                keyboardKeyRelease[i] = false;
            else
                keyReleasedLastFrame[i] = false;
        }

    for (unsigned int i = 0; i < inputs::Controller_MaxCount; i++)
        if (controllerConnected[i])
        {
            {
                // Stick and trigger axis
                int count;
                const float* stickAxis = glfwGetJoystickAxes(i, &count);
                // In case the controller was disconnected and the events weren't polled yet
                if (count == 0)
                    continue;

                controllerStickAxis[i][0] = Vector2(stickAxis[0], -stickAxis[1]);
                controllerStickAxis[i][1] = Vector2(stickAxis[2], -stickAxis[5]);

                controllerTriggerAxis[i][0] = stickAxis[3];
                controllerTriggerAxis[i][1] = stickAxis[4];
            }

            {
                // Button states
                int count;
                const unsigned char* buttonStates = glfwGetJoystickButtons(i, &count);
                // In case the controller was disconnected and the events weren't polled yet
                if (count == 0)
                    continue;

                controllerButton[i][inputs::Controller_ButtonSonySquare] = buttonStates[0];
                controllerButton[i][inputs::Controller_ButtonSonyCross] = buttonStates[1];
                controllerButton[i][inputs::Controller_ButtonSonyCircle] = buttonStates[2];
                controllerButton[i][inputs::Controller_ButtonSonyTriangle] = buttonStates[3];
                controllerButton[i][inputs::Controller_ButtonSonyL1] = buttonStates[4];
                controllerButton[i][inputs::Controller_ButtonSonyR1] = buttonStates[5];
                controllerButton[i][inputs::Controller_ButtonSonyL2] = buttonStates[6];
                controllerButton[i][inputs::Controller_ButtonSonyR2] = buttonStates[7];
                controllerButton[i][inputs::Controller_ButtonSonyShare] = buttonStates[8];
                controllerButton[i][inputs::Controller_ButtonSonyOptions] = buttonStates[9];
                controllerButton[i][inputs::Controller_ButtonSonyL3] = buttonStates[10];
                controllerButton[i][inputs::Controller_ButtonSonyR3] = buttonStates[11];
                controllerButton[i][inputs::Controller_ButtonSonyHome] = buttonStates[12];
                controllerButton[i][inputs::Controller_ButtonSonyTouchPad] = buttonStates[13];
            }

            {
                // Directional pad states
                int count;
                const unsigned char* directionalPad = glfwGetJoystickHats(i, &count);
                // In case the controller was disconnected and the events weren't polled yet
                if (count == 0)
                    continue;

                controllerDirectionalPad[i] = directionalPad[0];
            }
        }
}
