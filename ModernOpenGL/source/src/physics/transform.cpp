#include "physics/transform.hpp"

#include "core/object.hpp"

Transform::~Transform()
{
    if (parent)
        std::erase(parent->children, this);

    for (Transform* child : children)
        delete child;
}

void Transform::Update()
{
    mLocal = Matrix4x4::TRS(position, rotation, scale);

    if (parent)
        mGlobal = parent->mGlobal * mLocal;
    else
        mGlobal = mLocal;

    for (Transform* child : children)
        child->Update();
}

void Transform::DrawObject()
{
    mObject.Draw();

    for (Transform* child : children)
        child->DrawObject();
}
