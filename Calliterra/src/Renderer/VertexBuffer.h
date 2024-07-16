#pragma once
#include "Renderer/VertexBufferLayout.h"

class VertexBuffer
{
public:
	virtual ~VertexBuffer() = default;
	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual void SetLayout(int index = 0) = 0;
	virtual void CreateLayout(const VertexBufferLayout& layout) = 0;
	virtual void CreateLayoutList(const std::vector<VertexBufferLayout>& layoutList) = 0;
};

