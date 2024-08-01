#pragma once

class ConstantBuffer
{
public:
	virtual ~ConstantBuffer() = default;
	virtual void Bind(std::optional<uint32_t> slot) = 0;
};
