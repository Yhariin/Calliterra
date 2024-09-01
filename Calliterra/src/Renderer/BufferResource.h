#pragma once

class GraphicsContext;

class BufferResource
{
public:
	virtual ~BufferResource() = default;
	virtual void BindAsBuffer() const = 0;
	virtual void BindAsBuffer(BufferResource*) const = 0;
	virtual void Clear() const = 0;
};
