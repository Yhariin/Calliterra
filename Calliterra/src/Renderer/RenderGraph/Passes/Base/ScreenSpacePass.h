#pragma once
#include "BindingPass.h"
#include "Sandbox/Components/ScreenSpaceQuad.h"

class ScreenSpacePass : public BindingPass
{
public:
	ScreenSpacePass(const std::string name);

	void Execute() const override;

private:
	ScreenSpaceQuad m_SSQuad;
};

