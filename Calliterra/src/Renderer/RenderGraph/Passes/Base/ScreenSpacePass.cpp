#include "pch.h"
#include "ScreenSpacePass.h"

ScreenSpacePass::ScreenSpacePass(const std::string name)
	: BindingPass(std::move(name))
{
}

void ScreenSpacePass::Execute() const
{
	m_SSQuad.Bind();
	m_SSQuad.Draw();
}
