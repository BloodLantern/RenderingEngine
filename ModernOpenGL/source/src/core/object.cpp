#include "core/object.hpp"

#include <ImGui/imgui.h>

Object::Object(const std::string &name)
    : name(name), transform(*this)
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
    {
        mesh->Draw(viewProjectionMatrix * transform.mGlobal);
    }

    for (Object* const child : mChildren)
        child->Draw(viewProjectionMatrix);
}

void Object::DeleteChildren()
{
    for (Object* const child : mChildren)
    {
        child->DeleteChildren();
        delete child;
    }
}
