#include "low_renderer/camera.hpp"

#include "core/input.hpp"

void Camera::Update(const float deltaTime)
{
    Object::Update(deltaTime);

    if (isLookingAt)
        mEffectiveLookAt = lookingAt;
    else
        mEffectiveLookAt = /*(Vector3)
            ((Matrix4x4::Translation3D(Vector3::UnitZ())
            * Matrix3x3::Rotation3D(transform.rotation))
            * (Vector4) transform.position)*/transform.position + Vector3::UnitZ();

    const Vector3 forward = -(transform.position - mEffectiveLookAt).Normalized();
    const Vector3 right = forward.Cross(Vector3::UnitY()).Normalized();
    const float speed = moveSpeed * deltaTime;

    if (Input::keyboardKeyDown[inputs::KeyboardKey_W])
        transform.position += forward * speed;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_S])
        transform.position -= forward * speed;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_A])
        transform.position += right * speed;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_D])
        transform.position -= right * speed;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_Space])
        transform.position += Vector3::UnitY() * speed;
    if (Input::keyboardKeyDown[inputs::KeyboardKey_LeftShift])
        transform.position -= Vector3::UnitY() * speed;

    if (!isLookingAt)
        mEffectiveLookAt = transform.position + Vector3::UnitZ();

    Matrix4x4::ViewMatrix(transform.position, mEffectiveLookAt, Vector3::UnitY(), mView);
    Matrix4x4::PerspectiveProjectionMatrix(fov, 16 / 9.f, near, far, mProjection);
    mViewProjection = mProjection * mView;
}
