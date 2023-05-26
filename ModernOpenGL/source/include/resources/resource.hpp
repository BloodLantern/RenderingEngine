#pragma once

#include <filesystem>

class Resource
{
public:
    Resource() {}
    Resource(const std::filesystem::path&) {}
    virtual ~Resource() {}
};
