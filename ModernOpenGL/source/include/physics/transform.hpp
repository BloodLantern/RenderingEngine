#pragma once

#include <matrix4x4.hpp>
#include <vector3.hpp>

#include <vector>

class Object;

class Transform
{
public:
    Vector3 position, rotation, scale;

    Transform* parent = nullptr;
    std::vector<Transform*> children;

    Transform(Object& object, const Vector3& position = 0, const Vector3& rotation = 0, const Vector3& scale = 1)
        : position(position), rotation(rotation), scale(scale), mObject(object) {}
    ~Transform();

    void Update();
    void DrawObject();

    Object& GetObject() const { return mObject; }

protected:
    Object& mObject;
    Matrix4x4 mLocal, mGlobal;
};
