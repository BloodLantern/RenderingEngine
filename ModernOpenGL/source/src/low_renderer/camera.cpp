#include "low_renderer/camera.hpp"

#include "core/input.hpp"

void Camera::Update(const float deltaTime)
{
    Object::Update(deltaTime);

    Input::SetCursorHidden(hideCursor);

    if (hideCursor && Input::keyboardKeyPress[inputs::KeyboardKey_Escape])
        hideCursor = false;

    const float speed = moveSpeed * deltaTime;
    const float effectiveSensitivity = sensitivity * deltaTime;

    if (Input::controllerConnected[inputs::Controller_0])
    {
        const Vector2* const& sticks = Input::controllerStickAxis[inputs::Controller_0];
        const float* const& triggers = Input::controllerTriggerAxis[inputs::Controller_0];
        const bool* const& buttons = Input::controllerButtonDown[inputs::Controller_0];

        const Vector2 leftStick = sticks[inputs::Controller_StickLeft];
        const Vector3 movementForward = Vector3(mForward.x, 0, mForward.z).Normalized();
        transform.position -= movementForward * -leftStick.y * speed;
        transform.position -= movementForward.Cross(Vector3::UnitY()).Normalized() * leftStick.x * speed;

        const Vector2 rightStick = sticks[inputs::Controller_StickRight];
        transform.rotation.x += -rightStick.x * effectiveSensitivity;
        transform.rotation.y = std::clamp(
            transform.rotation.y + rightStick.y * effectiveSensitivity,
            -(std::numbers::pi_v<float> / 2) + std::numbers::pi_v<float> / 180,
            std::numbers::pi_v<float> / 2 - std::numbers::pi_v<float> / 180
        );

        fov = std::clamp(
            fov + (triggers[inputs::Controller_TriggerLeft] - triggers[inputs::Controller_TriggerRight])
                * effectiveSensitivity,
            std::numbers::pi_v<float> / 6,
            std::numbers::pi_v<float> / 1.5f
        );

        if (buttons[inputs::Controller_ButtonSonyCross])
            transform.position += Vector3::UnitY() * speed;
        if (buttons[inputs::Controller_ButtonSonyCircle])
            transform.position -= Vector3::UnitY() * speed;

        isLookingAt = buttons[inputs::Controller_ButtonSonyR1];
    }

    if (isLookingAt)
    {
        mForward = -(transform.position - lookingAt).Normalized();

        transform.rotation = Matrix3x3(
            1, 0, mForward.x,
            0, 1, mForward.y,
            0, 0, mForward.z
        ) * Vector3(1);
    }
    else
        mForward = transform.position + Vector3(
            std::cos(transform.rotation.x) * std::cos(transform.rotation.y),
            std::sin(transform.rotation.y),
            std::sin(transform.rotation.x) * std::cos(transform.rotation.y)
        ).Normalized();
    mRight = mForward.Cross(Vector3::UnitY()).Normalized();
    mUp = mRight.Cross(mForward).Normalized();

    /*if (Input::keyboardKeyDown[inputs::KeyboardKey_W])
        transform.position += mForward * speed;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_S])
        transform.position -= mForward * speed;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_A])
        transform.position += right * speed;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_D])
        transform.position -= right * speed;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_Space])
        transform.position += Vector3::UnitY() * speed;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_LeftShift])
        transform.position -= Vector3::UnitY() * speed;*/

    Matrix4x4::ViewMatrix(transform.position, mForward, Vector3::UnitY(), mView);
    Matrix4x4::PerspectiveProjectionMatrix(fov, 16 / 9.f, near, far, mProjection);
    mViewProjection = mProjection * mView;
}
