#pragma once
#include "GraphicsContext.h"

class Drawable;

class Bindable
{
public:
	virtual ~Bindable() = default;

	virtual void Bind() const = 0;
	virtual void InitializeParentReference(const Drawable&) {}
};
