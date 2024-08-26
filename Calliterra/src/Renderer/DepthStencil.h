#pragma once
#include "Bindable.h"

class DepthStencil : public Bindable
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

	virtual ~DepthStencil() = default;

	static const std::string GenerateUID(Mode mode);
	static std::shared_ptr<DepthStencil> Resolve(Mode mode);
	
};

