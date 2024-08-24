#include "pch.h"
#include "Cube.h"

Cube::Cube(DX::XMMATRIX transform, DX::XMFLOAT3 color)
	: Drawable(transform, color)
{ 
}


void Cube::InitBuffers()
{
	const auto geometryTag = "$Cube.";
	CalculateNormals();
	s_VertexShader = Shader::Resolve("assets/shaders/BPhongTexVS.hlsl", Shader::VERTEX_SHADER);
	s_PixelShader = Shader::Resolve("assets/shaders/BPhongTexPS.hlsl", Shader::PIXEL_SHADER);
	s_VertexBuffer = VertexBuffer::Resolve(geometryTag, m_IndependentCubeVertices, s_VertexShader.get());
	s_IndexBuffer = IndexBuffer::Resolve(geometryTag, m_IndependentCubeIndices);

	s_VertexShader->Bind();
	s_PixelShader->Bind();

	s_VertexBuffer->CreateLayout({
		{"POSITION", 0, ShaderDataType::Float3},
		{"NORMAL", 0, ShaderDataType::Float3},
		{"TEXCOORD", 0, ShaderDataType::Float2},
		});
	s_VertexBuffer->SetLayout();

	s_Texture = Texture::Resolve("assets/textures/brickwall.jpg");
		
	s_TransformConstantBuffer = ConstantBuffer::Resolve<CubeTransformConstantBuffer>(Shader::VERTEX_SHADER, {});

	CubePixelConstantBuffer pcb = {
		0.6f,
		128.f
	};

	s_PixelConstantBuffer = ConstantBuffer::Resolve<CubePixelConstantBuffer>(Shader::PIXEL_SHADER, pcb, 1);

	s_Blender = Blender::Resolve(false, Blender::BlendFunc::NONE, Blender::BlendFunc::NONE, Blender::BlendOp::NONE);
}

void Cube::Update(float dt)
{

}

void Cube::Draw()
{
	CubeTransformConstantBuffer cb = { 
		DX::XMMatrixTranspose(m_Transform * m_ViewMatrix),
		DX::XMMatrixTranspose(m_Transform * m_ViewMatrix * m_ProjectionMatrix),
		DX::XMMatrixInverse(nullptr, m_Transform * m_ViewMatrix)
	};

	Renderer::UpdateConstantBuffer(s_TransformConstantBuffer, cb);
	Renderer::Bind({ s_VertexShader, s_PixelShader }, s_VertexBuffer, s_IndexBuffer, { s_Texture }, { s_TransformConstantBuffer, s_PixelConstantBuffer }, s_Blender);
	Renderer::Draw();

}

void Cube::CalculateNormals()
{
	using namespace DirectX; // For some reason we need to include this line in order to use the XMMath overloaded operators...
	for (int i = 0; i < m_IndependentCubeIndices.size(); i += 3)
	{
		CubeVertex& v0 = m_IndependentCubeVertices[m_IndependentCubeIndices[i]];
		CubeVertex& v1 = m_IndependentCubeVertices[m_IndependentCubeIndices[i+1]];
		CubeVertex& v2 = m_IndependentCubeVertices[m_IndependentCubeIndices[i+2]];
		const XMVECTOR p0 = XMLoadFloat3(&v0.Position);
		const XMVECTOR p1 = XMLoadFloat3(&v1.Position);
		const XMVECTOR p2 = XMLoadFloat3(&v2.Position);

		const XMVECTOR normal = XMVector3Normalize(XMVector3Cross( (p1 - p0), (p2 - p0)) );

		XMStoreFloat3(&v0.Normal, normal);
		XMStoreFloat3(&v1.Normal, normal);
		XMStoreFloat3(&v2.Normal, normal);
	}
}
