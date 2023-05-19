#pragma once

#include "resources/resource.hpp"

class Shader : public Resource
{
public:
    Shader();
    Shader(const std::filesystem::path& path);

    bool Link();

    bool SetVertexShader(const std::filesystem::path& path);
    bool SetFragmentShader(const std::filesystem::path& path);
};
