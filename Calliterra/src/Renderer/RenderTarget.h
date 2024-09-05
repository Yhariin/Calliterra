#pragma once
#include "DepthStencilBuffer.h"
#include "BufferResource.h"
#include "Bindable.h"

class RenderTarget : public Bindable, public BufferResource
{
public:
	virtual ~RenderTarget() = default;
	
	//void Bind() const override {}
};

// Render target for binding as a texture slot for shaders
class ShaderInputRenderTarget : public RenderTarget
{
public:
	ShaderInputRenderTarget(uint32_t slot)
		: m_Slot(slot)
	{
	}


protected:
	uint32_t m_Slot;
};

// Render target used by the graphics context for the back buffer
class OutputOnlyRenderTarget : public RenderTarget
{
public:
	OutputOnlyRenderTarget() = default;

};
