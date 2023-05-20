#include "resources/resource_manager.hpp"

std::unordered_map<std::string, Resource*> ResourceManager::mResources;

void ResourceManager::Unload(const std::string& name)
{
    auto&& it = mResources.find(name);
    if (it == mResources.end())
        return;

    delete it->second;
    mResources.erase(it);
}

void ResourceManager::UnloadAll()
{
    for (std::pair<const std::string, Resource *>& it : mResources)
        delete it.second;
    mResources.clear();
}
