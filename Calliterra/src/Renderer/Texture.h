#pragma once
#include "Bindable.h"

class Renderer;

class Texture : public Bindable
{
public:
	enum class Filter
	{
		Point,
		Linear,
		Anisotropic
	};

	enum class TextureType
	{
		Texture2D,
		CubeMap
	};

public:
	virtual ~Texture() = default;

	virtual bool HasBlending() = 0;

	static const std::string GenerateUID(const std::string& filepath, uint32_t slot = 0, Filter filter = Filter::Anisotropic, TextureType type = TextureType::Texture2D);
	static std::shared_ptr<Texture> Resolve(const std::string& filepath, uint32_t slot = 0, Filter filter = Filter::Anisotropic, TextureType type = TextureType::Texture2D);

	
};
