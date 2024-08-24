#include "pch.h"
#include "DepthStencil.h"
#include "Renderer.h"

const std::string DepthStencil::GenerateUID(Mode mode)
{
    using namespace std::string_literals;
    return typeid(DepthStencil).name() + "#"s + std::to_string(static_cast<int>(mode));
}

std::shared_ptr<DepthStencil> DepthStencil::Resolve(Mode mode)
{
	return Renderer::GetResourceLibrary().Resolve<DepthStencil>(mode);
}
