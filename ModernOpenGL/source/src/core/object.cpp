#include "low_renderer/object.hpp"

Object::Object(const std::string &name)
    : name(name), transform(*this)
{
}

Object::Object(Mesh *const mesh, const std::string &name)
    : name(name), transform(*this), mesh(mesh)
{
}
