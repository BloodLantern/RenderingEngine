#pragma once

#include "core/object.hpp"

class Camera : public Object
{
public:
    bool isLookingAt = false;
    Vector3 lookingAt;
    float fov = 90;
    float near = 0.1f, far = 1000.f;
    float moveSpeed = 1.f;

    Camera() : Object("Camera") { transform.position = { 0, 0.5f, -1 }; }

    void Update(const float deltaTime) override;

    const Matrix4x4& GetViewProjectionMatrix() const { return mViewProjection; }
    const Vector3& GetEffectiveLookAt() const { return mEffectiveLookAt; }

private:
    Vector3 mEffectiveLookAt;
    Matrix4x4 mView, mProjection, mViewProjection;
};
