#include "pch.h"
#include "Model.h"

Model::Model(std::shared_ptr<rapidobj::Result> model, DX::XMMATRIX transform, DX::XMFLOAT3 color)
	: Drawable(transform, color)
{
	m_Vertices = ModelLoader::GetModelVertexVector(*model.get());
	m_Indices = ModelLoader::GetModelIndexVector(*model.get());

	InitBuffers();
}

Model::Model(std::shared_ptr<fastgltf::Asset> model, DX::XMMATRIX transform, DX::XMFLOAT3 color)
	: Drawable(transform, color)
{
	m_Vertices = ModelLoader::GetModelVertexVector(*model.get());
	m_Indices = ModelLoader::GetModelIndexVector(*model.get());

	InitBuffers();
}

Model::Model(std::shared_ptr<UfbxScene> model, DX::XMMATRIX transform, DX::XMFLOAT3 color)
	: Drawable(transform, color)
{
	m_Vertices = ModelLoader::GetModelVertexVector(*model.get());
	m_Indices = ModelLoader::GetModelIndexVector(*model.get(), m_Vertices);

	InitBuffers();
}

void Model::Draw()
{
	TransformConstantBuffer cb = { 
		DX::XMMatrixTranspose(m_Transform), 
		DX::XMMatrixTranspose(m_Transform * m_ViewMatrix),
		DX::XMMatrixTranspose(m_Transform * m_ViewMatrix * m_ProjectionMatrix),
		DX::XMMatrixInverse(nullptr, m_Transform * m_ViewMatrix)
	};

	Renderer::UpdateConstantBuffer(m_TransformConstantBuffer, cb);
	Renderer::Bind({ m_VertexShader, m_PixelShader }, m_VertexBuffer, m_IndexBuffer, { m_TransformConstantBuffer, m_PixelConstantBuffer });
	Renderer::Draw();

}

void Model::Update(DX::XMMATRIX transform)
{
	m_Transform = transform;
}

void Model::Update(float dt)
{
}

void Model::InitBuffers()
{
	m_VertexShader = Renderer::CreateShader("assets/shaders/PhongVS.hlsl", Shader::VERTEX_SHADER);
	m_PixelShader = Renderer::CreateShader("assets/shaders/PhongPS.hlsl", Shader::PIXEL_SHADER);
	m_VertexBuffer = Renderer::CreateVertexBuffer(m_Vertices, static_cast<uint32_t>(m_Vertices.size()), m_VertexShader.get());
	m_IndexBuffer = Renderer::CreateIndexBuffer(&m_Indices[0], static_cast<uint32_t>(m_Indices.size()));

	m_VertexShader->Bind();
	m_PixelShader->Bind();

	m_VertexBuffer->CreateLayout({
		{"POSITION", 0, ShaderDataType::Float3},
		{"NORMAL", 0, ShaderDataType::Float3},
		});
	m_VertexBuffer->SetLayout();

	m_TransformConstantBuffer = Renderer::CreateConstantBuffer<TransformConstantBuffer>(Shader::VERTEX_SHADER);

	PixelConstantBuffer pcb = {
		m_FlatColor,
		.3f,
		32.f
	};
	m_PixelConstantBuffer = Renderer::CreateConstantBuffer<PixelConstantBuffer>(Shader::PIXEL_SHADER, pcb, 1);

}
