#pragma once

#include <GLFW/glfw3.h>

#include <vector2.hpp>
#include <vector2i.hpp>

#include "core/input_constants.hpp"

class Input // Static class used to get keyboard, mouse and controller input.
{
public:
    static Vector2i mousePosition; // Mouse position Vector2.
    static bool mouseDown[inputs::MouseButton_MaxCount]; // Mouse button down state array.
    static bool mouseRelease[inputs::MouseButton_MaxCount]; // Mouse button release state array.
    static Vector2 mouseWheel; // Mouse wheel offset Vector2. Includes keyboard wheel.

    static bool keyboardKeyDown[inputs::KeyboardKey_MaxCount]; // Keyboard key down state array.
    static bool keyboardKeyRelease[inputs::KeyboardKey_MaxCount]; // Keyboard key release state array.

    static unsigned char controllerConnectedCount; // Controller connected count. Ranges from 0 to 16.
    static bool controllerConnected[inputs::Controller_MaxCount]; // Controller connected state array.
    static Vector2 controllerStickAxis[inputs::Controller_MaxCount][inputs::Controller_StickCount]; // Controller stick axis array. Ranges from -1 to 1.
    static float controllerTriggerAxis[inputs::Controller_MaxCount][inputs::Controller_TriggerCount]; // Controller trigger axis array. Ranges from -1 to 1, -1 being not pressed and 1 being fully pressed.
    static bool controllerButton[inputs::Controller_MaxCount][inputs::Controller_ButtonCount]; // Controller button states array.
    static unsigned char controllerDirectionalPad[inputs::Controller_MaxCount]; // Controller directional pad (D-Pad) value array. Values are bits and therefore should be used with bitwise operators.

    // You can't instantiate this class.
    Input() = delete;

    static void Initialize(GLFWwindow* const window); // Function used to setup OpenGL callbacks. Automatically called.
    static void Update(); // Function used to update the input states. Automatically called.
};
