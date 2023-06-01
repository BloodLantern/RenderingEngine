#pragma once

#include "core/data_structure/transform.hpp"
#include "low_renderer/mesh.hpp"

#define OBJECT_DEFAULT_NAME "Object"

class Object
{
    friend class ObjectHierarchy;

public:
    std::string name;
    Transform transform;
    Mesh* mesh = nullptr;

    Object(const std::string& name = OBJECT_DEFAULT_NAME);
    Object(const Vector3& position, const std::string& name = OBJECT_DEFAULT_NAME);
    Object(Mesh* const mesh, const std::string& name = OBJECT_DEFAULT_NAME);
    ~Object();

    virtual void Update(const float deltaTime);
    void Draw(const Matrix4x4& viewProjectionMatrix);

    void SetParent(Object* const o) { mParent = o; }

    void AddChild(Object* const o);
    void AddChildren(const std::vector<Object*>& o);
    void RemoveChild(Object* const o);
    void RemoveAllChildren();
    void ForEachChild(std::function<void(Object* const)> f);

    void DeleteChildren();

private:
    Object* mParent = nullptr;
    std::vector<Object*> mChildren;
};
