#pragma once
#include "Bindable.h"
#include "BufferResource.h"

class DepthStencilBuffer : public Bindable, public BufferResource
{
public:
	void Bind() const override {}
};
