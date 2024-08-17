#include "pch.h"
#include "Shader.h"
#include "Renderer.h"

const std::string Shader::GenerateUID(const std::string& filepath, Shader::ShaderType type)
{
	using namespace std::string_literals;
	return typeid(Shader).name() + "#"s + filepath + "#"s + std::to_string(static_cast<int>(type)); 
}

std::shared_ptr<Shader> Shader::Resolve(const std::string& filepath, Shader::ShaderType type)
{
	return Renderer::GetResourceLibrary().Resolve<Shader>(filepath, type);
}
