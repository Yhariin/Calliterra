#include "pch.h"
#include "DepthStencilMask.h"
#include "Renderer.h"

const std::string DepthStencilMask::GenerateUID(Mode mode)
{
    using namespace std::string_literals;
    return typeid(DepthStencilMask).name() + "#"s + std::to_string(static_cast<int>(mode));
}

std::shared_ptr<DepthStencilMask> DepthStencilMask::Resolve(Mode mode)
{
	return Renderer::GetResourceLibrary().Resolve<DepthStencilMask>(mode);
}
