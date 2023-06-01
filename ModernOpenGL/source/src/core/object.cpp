#include "core/object.hpp"

#include <ImGui/imgui.h>

Object::Object(const std::string &name)
    : name(name), transform(*this)
{
}

Object::Object(const Vector3 &position, const std::string &name)
    : name(name), transform(*this, position)
{
}

Object::Object(Mesh *const mesh, const std::string &name)
    : name(name), transform(*this), mesh(mesh)
{
}

Object::~Object()
{
    if (mParent)
        std::erase(mParent->mChildren, this);
}

void Object::Update(const float deltaTime)
{
    transform.Update(mParent ? &mParent->transform : nullptr);

    for (Object* const child : mChildren)
        child->Update(deltaTime);
}

void Object::Draw(const Matrix4x4& viewProjectionMatrix)
{
    if (mesh)
        mesh->Draw(transform.mGlobal, viewProjectionMatrix);

    for (Object* const child : mChildren)
        child->Draw(viewProjectionMatrix);
}
void Object::AddChild(Object *const o)
{
    o->mParent = this;
    mChildren.push_back(o);
}

void Object::AddChildren(const std::vector<Object *> &o)
{
    for (Object *const child : o)
        child->mParent = this;
    mChildren.insert(mChildren.end(), o.begin(), o.end());
}

void Object::RemoveChild(Object *const o)
{
    o->mParent = nullptr;
    mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), o));
}

void Object::RemoveAllChildren()
{
    ForEachChild([] (Object *const o) { o->mParent = nullptr; });
    mChildren.clear();
}

void Object::ForEachChild(std::function<void(Object *const)> f)
{
    for (Object* const child : mChildren)
        f(child);
}

void Object::DeleteChildren()
{
    for (Object* const child : mChildren)
    {
        child->DeleteChildren();
        delete child;
    }
}
