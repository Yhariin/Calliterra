#pragma once
#include "DepthStencilBuffer.h"
#include "BufferResource.h"
#include "Bindable.h"

class RenderTarget : public Bindable, public BufferResource
{
public:
	virtual ~RenderTarget() = default;
	
	virtual void BindAsTexture(uint32_t slot = 0) const = 0;
	void Bind() const override {}
};

