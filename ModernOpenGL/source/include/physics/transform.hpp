#pragma once

#include <matrix4x4.hpp>
#include <vector3.hpp>

#include <vector>
#include <functional>

class Object;

class Transform
{
public:
    Vector3 position, rotation, scale;

    Transform(Object& object, const Vector3& position = 0, const Vector3& rotation = 0, const Vector3& scale = 1)
        : position(position), rotation(rotation), scale(scale), mObject(object) {}
    ~Transform();

    void Update(const Transform* parent);

    Object& GetObject() const { return mObject; }

protected:
    Object& mObject;
    Matrix4x4 mLocal, mGlobal;
};
