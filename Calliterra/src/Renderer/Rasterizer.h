#pragma once
#include "Bindable.h"

enum class FillMode
{
	Solid,
	Wireframe
};

enum class CullMode
{
	None,
	Front,
	Back
};

class Rasterizer : public Bindable
{
public:
	virtual ~Rasterizer() = default;

	static const std::string GenerateUID(FillMode fillMode, CullMode cullMode);
	static std::shared_ptr<Rasterizer> Resolve(FillMode fillMode, CullMode cullMode);
};
