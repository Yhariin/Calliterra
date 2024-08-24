#include "pch.h"
#include "Blender.h"
#include "Renderer.h"

const std::string Blender::GenerateUID(bool enableBlending, BlendFunc srcBlend, BlendFunc destBlend, BlendOp blendOp)
{
    using namespace std::string_literals;
    return typeid(Blender).name() + "#"s + (enableBlending ? "Enabled"s : "Disabled"s) + "#"s + 
        std::to_string(static_cast<int>(srcBlend)) + std::to_string(static_cast<int>(destBlend)) + std::to_string(static_cast<int>(blendOp));
}

std::shared_ptr<Blender> Blender::Resolve(bool enableBlending, BlendFunc srcBlend, BlendFunc destBlend, BlendOp blendOp)
{
	return Renderer::GetResourceLibrary().Resolve<Blender>(enableBlending, srcBlend, destBlend, blendOp);
}
