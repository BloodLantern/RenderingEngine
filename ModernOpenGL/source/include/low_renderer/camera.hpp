#pragma once

#include "core/object.hpp"

#include <numbers>

class Camera : public Object
{
public:
    bool isLookingAt = false;
    Vector3 lookingAt;
    float fov = std::numbers::pi_v<float> / 2;
    float near = 0.1f, far = 1000.f;
    float moveSpeed = 1.f;
    float sensitivity = 5.f;
    bool hideCursor = false;

    Camera() : Object("Camera") { transform.position = { 0, 0.5f, -1 }; }

    void Update(const float deltaTime) override;

    const Matrix4x4& GetViewProjectionMatrix() const { return mViewProjection; }
    const Vector3& GetForward() const { return mForward; }

private:
    Vector3 mForward, mRight, mUp;
    Matrix4x4 mView, mProjection, mViewProjection;
};
