#include "pch.h"
#include "Rasterizer.h"
#include "Renderer.h"

const std::string Rasterizer::GenerateUID(FillMode fillMode, CullMode cullMode)
{
    using namespace std::string_literals;
    return typeid(Rasterizer).name() + "#"s + std::to_string(static_cast<int>(fillMode)) + "#"s + std::to_string(static_cast<int>(cullMode));
}

std::shared_ptr<Rasterizer> Rasterizer::Resolve(FillMode fillMode, CullMode cullMode)
{
	return Renderer::GetResourceLibrary().Resolve<Rasterizer>(fillMode, cullMode);
}
