#include "low_renderer/camera.hpp"

void Camera::Update(const float deltaTime)
{
    Object::Update(deltaTime);

    Matrix4x4::ViewMatrix(transform.position, lookAt, Vector3::UnitY(), mView);
    Matrix4x4::ProjectionMatrix(fov, 16 / 9.f, near, far, mProjection);
    mViewProjection = mProjection * mView;
}
