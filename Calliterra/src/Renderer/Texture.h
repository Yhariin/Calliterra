#pragma once

class Texture
{
public:
	virtual ~Texture() = default;

	virtual void Bind(std::optional<uint32_t> slot) = 0;
	
};
