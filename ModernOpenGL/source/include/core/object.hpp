#pragma once

#include "physics/transform.hpp"
#include "low_renderer/mesh.hpp"

class Object
{
public:
    std::string name;
    Transform transform;
    Mesh* mesh;

    Object(const std::string& name = "");
    Object(Mesh* const mesh = nullptr, const std::string& name = "");
    ~Object();

    void Update(const float deltaTime);
    void Draw();

    void SetParent(Object* const o) { mParent = o; }

    void AddChild(Object* const o) { mChildren.push_back(o); }
    void AddChildren(const std::vector<Object*>& o) { mChildren.insert(mChildren.end(), o.begin(), o.end()); }
    void RemoveChild(Object* const o) { mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), o)); }
    void RemoveAllChildren() { mChildren.clear(); }
    void ForEachChild(std::function<void(Object* const)> f) { for (Object* const child : mChildren) f(child); }

private:
    Object* mParent = nullptr;
    std::vector<Object*> mChildren;
};
