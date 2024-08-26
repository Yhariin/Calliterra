#pragma once
#include "GraphicsContext.h"

class Bindable
{
public:
	virtual ~Bindable() = default;

	virtual void Bind() = 0;
};
