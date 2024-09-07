#include "pch.h"
#include "Texture.h"
#include "Renderer.h"

const std::string Texture::GenerateUID(const std::string& filepath, uint32_t slot, Filter filter, TextureType type)
{
	using namespace std::string_literals;
	return typeid(Texture).name() + "#"s + filepath + "#" + std::to_string(slot) + "#" + std::to_string(static_cast<int>(filter)) + "#" + std::to_string(static_cast<int>(type));
}

std::shared_ptr<Texture> Texture::Resolve(const std::string& filepath, uint32_t slot, Filter filter, TextureType type)
{
	return Renderer::GetResourceLibrary().Resolve<Texture>(filepath, slot, filter, type);
}
