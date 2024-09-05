#include "pch.h"
#include "FullScreenQuad.h"
#include "Renderer/Renderer.h"

FullScreenQuad::FullScreenQuad()
{
		std::string tag = "$ScreenSpaceQuad";

		m_VertexShader = Shader::Resolve("assets/shaders/FullScreenQuadVS.hlsl", Shader::VERTEX_SHADER);

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

void FullScreenQuad::Bind() const
{

	std::vector<std::shared_ptr<Bindable>> bindables = {
		m_VertexBuffer,
		m_IndexBuffer,
		m_VertexShader,
	};

	Renderer::Bind(bindables, m_IndexBuffer->GetCount());
}

void FullScreenQuad::Draw() const
{
	Renderer::Draw();
}
