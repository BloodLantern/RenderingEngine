#pragma once

#include "resources/resource.hpp"

class Texture : public Resource
{
public:
	Texture(const std::string& filepath) : Resource(filepath) {}

	// Inherited via Resource
	void Load(const std::filesystem::path& filepath) override;
	void Unload() override;
};
