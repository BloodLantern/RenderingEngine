#include "core/data_structure/transform.hpp"

#include "core/object.hpp"

void Transform::Update(const Transform* parent)
{
    mLocal = Matrix4x4::TRS(position, rotation, scale);

    if (parent)
        mGlobal = mLocal * parent->mGlobal;
    else
        mGlobal = mLocal;
}
