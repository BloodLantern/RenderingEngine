#pragma once

#include "resources/resource.hpp"

#include <glad/glad.h>

class Texture : public Resource
{
public:
    Texture() {}
    Texture(const std::filesystem::path& filepath) : Resource(filepath) { Load(filepath); }
    ~Texture() { Unload(); }

    void Load(const std::filesystem::path& filepath);
    void Unload() { glDeleteTextures(1, &mId); }

    void Use() { glBindTexture(GL_TEXTURE_2D, mId); }

private:
    unsigned int mId;
};
