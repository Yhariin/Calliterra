#pragma once
#include "Renderer/Shader.h"
#include "Platform/DX11/DX11Context.h"

class DX11Shader : public Shader
{
public:
	DX11Shader(DX11Context& context, const std::string& filepath, Shader::ShaderType type);

	void Bind() override;
	void Unbind() override;

	ComPtr<ID3DBlob> GetCompiledShaderByteCode() { return m_CompiledShader; }
private:
	const std::string ShaderTypeToCompilerTarget();

private:
	DX11Context& m_DX11Context;
	std::string m_Filepath;
	ShaderType m_ShaderType;
	ComPtr<ID3DBlob> m_CompiledShader;
	ComPtr<ID3DBlob> m_CompilationErrorMsgs;

	ComPtr<ID3D11VertexShader> m_VertexShader;
	ComPtr<ID3D11PixelShader> m_PixelShader;
	ComPtr<ID3D11ComputeShader> m_ComputeShader;
	ComPtr<ID3D11GeometryShader> m_GeometryShader;
	ComPtr<ID3D11HullShader> m_HullShader;
	ComPtr<ID3D11DomainShader> m_DomainShader;
};

