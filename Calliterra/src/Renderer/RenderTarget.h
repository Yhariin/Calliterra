#pragma once
#include "DepthStencil.h"

class RenderTarget
{
public:
	virtual ~RenderTarget() = default;
	
	virtual void Clear() const = 0;

	virtual void BindAsTexture(uint32_t slot = 0) const = 0;
	virtual void BindAsTarget() const = 0;
	virtual void BindAsTargetDepth() const = 0;

};

