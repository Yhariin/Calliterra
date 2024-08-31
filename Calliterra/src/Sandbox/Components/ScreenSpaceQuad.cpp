#include "pch.h"
#include "ScreenSpaceQuad.h"
#include "Renderer/Renderer.h"

ScreenSpaceQuad::ScreenSpaceQuad()
{
		std::string tag = "$ScreenSpaceQuad";

		m_VertexShader = Shader::Resolve("assets/shaders/SSQuadVS.hlsl", Shader::VERTEX_SHADER);
		m_PixelShader = Shader::Resolve("assets/shaders/ColorInvertPS.hlsl", Shader::PIXEL_SHADER);

		std::vector<DX::XMFLOAT2> vBuff =
		{
			{ -1,  1},
			{  1,  1},
			{ -1, -1},
			{  1, -1}
		};

		m_VertexBuffer = VertexBuffer::Resolve(tag, vBuff);
		m_VertexBuffer->CreateLayout({
			{"POSITION", 0, ShaderDataType::Float2}
			}, m_VertexShader.get());

		std::vector<uint32_t> iBuff = { 0, 1, 2, 1, 3, 2 };
		m_IndexBuffer = IndexBuffer::Resolve(tag, iBuff);

	}

void ScreenSpaceQuad::Bind()
{

	std::vector<std::shared_ptr<Bindable>> bindables = {
		m_VertexBuffer,
		m_IndexBuffer,
		m_VertexShader,
		m_PixelShader
	};

	Renderer::Bind(bindables, m_IndexBuffer->GetCount());
}

void ScreenSpaceQuad::Draw()
{
	Renderer::Draw();
}
