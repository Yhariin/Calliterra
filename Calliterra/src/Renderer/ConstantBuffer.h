#pragma once

class ConstantBuffer
{
public:
	virtual ~ConstantBuffer() = default;
	virtual void Bind() = 0;
};
