#pragma once

#include "core/object.hpp"

class Camera : public Object
{
public:
    Vector3 lookAt;
    float fov = 90;
    float near = 0.1f, far = 1000.f;

    Camera() : Object("Camera") {}

    void Update(const float deltaTime) override;

    const Matrix4x4& GetViewProjectionMatrix() const { return mViewProjection; }

private:
    Matrix4x4 mView, mProjection, mViewProjection;
};
