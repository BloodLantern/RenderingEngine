#pragma once

#include "resources/resource.hpp"

class Texture : public Resource
{
public:
    Texture() {}
    Texture(const std::filesystem::path& filepath) : Resource(filepath) { Load(filepath); }
    ~Texture() { Unload(); }

    void Load(const std::filesystem::path& filepath) override;
    void Unload() override;

    void Use();

private:
    unsigned int mId;
};
