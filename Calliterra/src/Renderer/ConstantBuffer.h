#pragma once
#include "Bindable.h"
#include "Shader.h"

class ConstantBuffer : public Bindable
{
public:
	virtual ~ConstantBuffer() = default;

	template <typename...IgnoreParams>
	static const std::string GenerateUID(const std::string& tag, Shader::ShaderType shaderType, IgnoreParams&&...ignore)
	{
		using namespace std::string_literals;
		return typeid(ConstantBuffer).name() + "#"s + tag + "#"s + std::to_string(shaderType);
	}

	template<typename Type>
	static std::shared_ptr<ConstantBuffer> Resolve(Shader::ShaderType shaderType, const Type& constants, uint32_t slot = 0, const std::string& tag = typeid(Type).name())
	{
		return Renderer::GetResourceLibrary().Resolve<ConstantBuffer>(tag, shaderType, constants, slot);
	}

	virtual void Bind(std::optional<uint32_t> slot) = 0;

protected:
};
