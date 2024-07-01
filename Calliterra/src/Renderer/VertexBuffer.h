#pragma once
#include "Renderer/VertexBufferLayout.h"

class VertexBuffer
{
public:
	virtual ~VertexBuffer() = default;
	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual void SetLayout(const VertexBufferLayout& layout) = 0;
	virtual const VertexBufferLayout& GetLayout() = 0;
};

