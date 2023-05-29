#pragma once

#include "core/data_structure/transform.hpp"
#include "low_renderer/mesh.hpp"

class Object
{
    friend class ObjectHierarchy;

public:
    std::string name;
    Transform transform;
    Mesh* mesh = nullptr;

    Object(const std::string& name = "");
    Object(Mesh* const mesh, const std::string& name = "");
    ~Object();

    virtual void Update(const float deltaTime);
    void Draw(const Matrix4x4& viewProjectionMatrix);

    void SetParent(Object* const o) { mParent = o; }

    void AddChild(Object* const o) { mChildren.push_back(o); }
    void AddChildren(const std::vector<Object*>& o) { mChildren.insert(mChildren.end(), o.begin(), o.end()); }
    void RemoveChild(Object* const o) { mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), o)); }
    void RemoveAllChildren() { mChildren.clear(); }
    void ForEachChild(std::function<void(Object* const)> f) { for (Object* const child : mChildren) f(child); }

    void DeleteChildren();

private:
    Object* mParent = nullptr;
    std::vector<Object*> mChildren;
};
