#pragma once

#include "resources/resource.hpp"
#include "resources/texture.hpp"

#include <type_traits>
#include <unordered_map>
#include <string>

template<class T>
concept Resource_T = std::is_base_of<Resource, T>::value;

class ResourceManager
{
public:
    // You cannot instantiate this class
    ResourceManager() = delete;

    template<Resource_T T>
    static T* Load(const std::string& name)
    {
        T* resource = new T(name);
        auto&& result = mResources.try_emplace(name, resource);
        // If a resource with the same name already exists
        if (!result.second)
        {
            // Delete the old resource
            delete result.first->second;
            // Remove it from the map
            mResources.erase(result.first);
            // And emplace the new one
            mResources.emplace(name, resource);
        }
        return resource;
    }

    template<Resource_T T>
    static T* Get(const std::string& name)
    {
        return mResources.contains(name) ? dynamic_cast<T*>(mResources[name]) : nullptr;
    }
    
    static void Unload(const std::string& name);

private:
    // As we don't care about the map to be sorted, std::unordered_map is used instead
    // as most of it functions are of constant to linear complexity
    static std::unordered_map<std::string, Resource*> mResources;
};
