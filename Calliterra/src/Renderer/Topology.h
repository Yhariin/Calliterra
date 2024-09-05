#pragma once
#include "Bindable.h"

enum class PrimitiveTopology
{
	None = 0,
	Points,
	Lines,
	Triangles,
	LineStrip,
	TriangleStrip,
};

class Topology : public Bindable
{
public:
	virtual ~Topology() = default;

	static const std::string GenerateUID(PrimitiveTopology primtiveTopology);
	static std::shared_ptr<Topology> Resolve(PrimitiveTopology primtiveTopology);
};
