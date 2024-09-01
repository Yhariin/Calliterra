#pragma once
#include "Bindable.h"

class DepthStencilMask : public Bindable
{
public:
	enum class Mode
	{
		Off = 0,
		Write,
		Mask,
		DepthOff,
		DepthReversed,
		DepthFirst
	};

	virtual ~DepthStencilMask() = default;

	static const std::string GenerateUID(Mode mode);
	static std::shared_ptr<DepthStencilMask> Resolve(Mode mode);

};
