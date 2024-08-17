#pragma once
#include "Bindable.h"

class Renderer;

class Texture : public Bindable
{
public:
	virtual ~Texture() = default;

	static const std::string GenerateUID(const std::string& filepath, uint32_t slot = 0);
	static std::shared_ptr<Texture> Resolve(const std::string& filepath, uint32_t slot = 0);

	virtual void Bind(std::optional<uint32_t> slot) = 0;
	
};
