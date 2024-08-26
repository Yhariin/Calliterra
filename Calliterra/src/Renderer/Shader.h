#pragma once
#include "Bindable.h"

class Renderer;

class Shader : public Bindable
{
public:
	enum ShaderType
	{
		UKNOWN = 0,
		VERTEX_SHADER,
		PIXEL_SHADER,
		COMPUTE_SHADER,
		GEOMETRY_SHADER,
		HULL_SHADER,
		DOMAIN_SHADER
	};

public:
	virtual ~Shader() = default;

	static const std::string GenerateUID(const std::string& filepath, Shader::ShaderType type);

	static std::shared_ptr<Shader> Resolve(const std::string& filepath, Shader::ShaderType type);
};
