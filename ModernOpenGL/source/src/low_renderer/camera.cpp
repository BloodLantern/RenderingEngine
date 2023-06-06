#include "low_renderer/camera.hpp"

#include "core/input.hpp"

#include <algorithm>

void Camera::Update(const float deltaTime)
{
    Object::Update(deltaTime);

    Input::SetCursorHidden(fpsView);

    if (fpsView && Input::keyboardKeyPress[inputs::KeyboardKey_Escape])
        fpsView = false;

    const float effectiveSpeed = moveSpeed * deltaTime;
    Vector3 movement;

    if (Input::controllerConnected[inputs::Controller_0])
    {
        const Vector2* const& sticks = Input::controllerStickAxis[inputs::Controller_0];
        const float* const& triggers = Input::controllerTriggerAxis[inputs::Controller_0];
        const bool* const& buttons = Input::controllerButtonDown[inputs::Controller_0];

        const Vector2 leftStick = sticks[inputs::Controller_StickLeft];
        movement = mForward * leftStick.y;
        movement -= mRight * leftStick.x;

        const float effectiveStickSensitivity = stickSensitivity / 100;
        const Vector2 rightStick = sticks[inputs::Controller_StickRight];
        transform.rotation.x -= rightStick.x * effectiveStickSensitivity;
        transform.rotation.y = std::clamp(
            transform.rotation.y + rightStick.y * effectiveStickSensitivity,
            -(std::numbers::pi_v<float> / 2) + std::numbers::pi_v<float> / 180,
            std::numbers::pi_v<float> / 2 - std::numbers::pi_v<float> / 180
        );

        fov = std::clamp(
            fov + (triggers[inputs::Controller_TriggerLeft] - triggers[inputs::Controller_TriggerRight])
                * effectiveStickSensitivity,
            std::numbers::pi_v<float> / 6,
            std::numbers::pi_v<float> / 2
        );

        if (buttons[inputs::Controller_ButtonSonyCross])
            transform.position += Vector3::UnitY() * effectiveSpeed;
        if (buttons[inputs::Controller_ButtonSonyCircle])
            transform.position -= Vector3::UnitY() * effectiveSpeed;

        isLookingAt = buttons[inputs::Controller_ButtonSonyR1];

        // FIXME: Use button press instead
        if (Input::controllerButtonDown[inputs::Controller_0][inputs::Controller_ButtonSonyL3])
            mRunning = true;
    }

    if (Input::keyboardKeyDown[inputs::KeyboardKey_W])
        movement += mForward;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_S])
        movement -= mForward;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_A])
        movement += mRight;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_D])
        movement -= mRight;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_Space])
        transform.position += Vector3::UnitY() * effectiveSpeed;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_LeftShift])
        transform.position -= Vector3::UnitY() * effectiveSpeed;
        
    if (Input::keyboardKeyPress[inputs::KeyboardKey_LeftControl])
        mRunning = true;

    movement.y = 0;
    if (movement == 0)
        mRunning = false;
    else
        transform.position += movement.Normalized() * effectiveSpeed * (mRunning ? 2.f : 1.f);

    if (fpsView)
    {
        const float effectiveMouseSensitivity = mouseSensitivity / 1000;
        const Vector2i mouseDelta = lastMousePosition - Input::mousePosition;
        transform.rotation.x += mouseDelta.x * effectiveMouseSensitivity;
        transform.rotation.y = std::clamp(
                transform.rotation.y + mouseDelta.y * effectiveMouseSensitivity,
                -(std::numbers::pi_v<float> / 2) + std::numbers::pi_v<float> / 180,
                std::numbers::pi_v<float> / 2 - std::numbers::pi_v<float> / 180
            );
        lastMousePosition = Input::mousePosition;

        fov = std::clamp(
            fov - Input::mouseWheel.y * 10 * std::numbers::pi_v<float> / 180,
            std::numbers::pi_v<float> / 6,
            std::numbers::pi_v<float> / 2
        );
    }

    if (transform.rotation.x > std::numbers::pi_v<float> * 2)
        transform.rotation.x -= std::numbers::pi_v<float> * 4;
    else if (transform.rotation.x < -std::numbers::pi_v<float> * 2)
        transform.rotation.x += std::numbers::pi_v<float> * 4;

    if (isLookingAt)
    {
        mForward = (lookingAt - transform.position).Normalized();
        transform.rotation = mForward * 2 * std::numbers::pi_v<float>;
    }
    else
        mForward = Vector3(
            std::cos(transform.rotation.x) * std::cos(transform.rotation.y),
            std::sin(transform.rotation.y),
            std::sin(transform.rotation.x) * std::cos(transform.rotation.y)
        ).Normalized();
    mRight = mForward.Cross(Vector3::UnitY()).Normalized();
    mUp = mRight.Cross(mForward).Normalized();

    Matrix4x4::ViewMatrix(transform.position, transform.position + mForward, Vector3::UnitY(), mView);
    Matrix4x4::PerspectiveProjectionMatrix(fov, 16 / 9.f, near, far, mProjection);
    mViewProjection = mProjection * mView;
}
