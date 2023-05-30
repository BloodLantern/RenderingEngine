#pragma once

#include <filesystem>

class Resource
{
public:
    Resource() = default;
    Resource(const std::filesystem::path&) {}
    virtual ~Resource() {}
};
