#pragma once

class Shader
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

	virtual void Bind() = 0;
	virtual void Unbind() = 0;
};
