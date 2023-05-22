#include "core/object.hpp"

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
void Object::Draw()
{
    if (mesh)
        mesh->Draw();

    for (Object* const child : mChildren)
        child->Draw();
}
