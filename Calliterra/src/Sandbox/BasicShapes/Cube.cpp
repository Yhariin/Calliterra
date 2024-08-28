#include "pch.h"
#include "Cube.h"

Cube::Cube(DX::XMMATRIX transform, DX::XMFLOAT3 color)
	: Drawable(transform, color)
{ 
	InitBuffers();
}


void Cube::InitBuffers()
{
	using namespace std::string_literals;
	const auto geometryTag = "$Cube.";
	const auto outlineTag = geometryTag + "Outline"s;
	CalculateNormals();

	{
		Technique standard;
		{
			Step only(0);

			m_VertexShader = Shader::Resolve("assets/shaders/BPhongTexVS.hlsl", Shader::VERTEX_SHADER);
			m_PixelShader = Shader::Resolve("assets/shaders/BPhongTexPS.hlsl", Shader::PIXEL_SHADER);
			m_VertexBuffer = VertexBuffer::Resolve(geometryTag, m_IndependentCubeVertices);
			m_IndexBuffer = IndexBuffer::Resolve(geometryTag, m_IndependentCubeIndices);

			m_VertexBuffer->CreateLayout({
				{"POSITION", 0, ShaderDataType::Float3},
				{"NORMAL", 0, ShaderDataType::Float3},
				{"TEXCOORD", 0, ShaderDataType::Float2},
				}, m_VertexShader.get());

			m_Texture = Texture::Resolve("assets/textures/brickwall.jpg");

			//m_TransformConstantBuffer = ConstantBuffer::Resolve<CubeTransformConstantBuffer>(Shader::VERTEX_SHADER, {});
			m_TransformConstantBuffer = std::make_shared<TransformConstantBuffer>();

			CubePixelConstantBuffer pcb = {
				0.6f,
				128.f
			};

			m_PixelConstantBuffer = ConstantBuffer::Resolve<CubePixelConstantBuffer>(Shader::PIXEL_SHADER, pcb, 1);

			m_Blender = Blender::Resolve(false, Blender::BlendFunc::NONE, Blender::BlendFunc::NONE, Blender::BlendOp::NONE);

			only.AddBindable(m_VertexShader);
			only.AddBindable(m_PixelShader);
			only.AddBindable(m_VertexBuffer);
			only.AddBindable(m_IndexBuffer);
			only.SetIndexCount(m_IndexBuffer->GetCount());
			only.AddBindable(m_Texture);
			only.AddBindable(m_TransformConstantBuffer);
			only.AddBindable(m_PixelConstantBuffer);
			only.AddBindable(m_Blender);

			standard.AddStep(std::move(only));
		}
		AddTechnique(std::move(standard));
	}
	{
		Technique outline;
		{
			Step mask(1);

			m_OutlineVS = Shader::Resolve("assets/shaders/FlatColorVS.hlsl", Shader::VERTEX_SHADER);
			m_OutlinePS = Shader::Resolve("assets/shaders/FlatColorPS.hlsl", Shader::PIXEL_SHADER);
			m_OutlineVertexBuffer = VertexBuffer::Resolve(outlineTag, m_CubeVertices);
			m_OutlineIndexBuffer = IndexBuffer::Resolve(outlineTag, m_CubeIndices);
			m_OutlineVertexBuffer->CreateLayout({
				{"POSITION", 0, ShaderDataType::Float3}
				}, m_OutlineVS.get());

			//m_TransformConstantBuffer = std::make_shared<TransformConstantBuffer>();

			mask.AddBindable(m_OutlineVS);
			mask.AddBindable(m_OutlineVertexBuffer);
			mask.AddBindable(m_OutlineIndexBuffer);
			mask.SetIndexCount(m_OutlineIndexBuffer->GetCount());
			mask.AddBindable(m_TransformConstantBuffer);

			outline.AddStep(std::move(mask));
		}
		{
			Step draw(2);

			class TransformCBuffScaling : public TransformConstantBuffer
			{
			public:
				using TransformConstantBuffer::TransformConstantBuffer;
				Transforms GetTransforms() const override
				{
					ASSERT(m_Parent != nullptr);
					auto m = m_Parent->GetTransform();
					auto v = m_Parent->GetViewTransform();
					auto p = m_Parent->GetProjectionTransform();

					auto newM = DX::XMMatrixScaling(1.03f, 1.03f, 1.03f) * m;
					return {
						DX::XMMatrixTranspose(newM * v),
						DX::XMMatrixTranspose(newM * v * p),
						DX::XMMatrixInverse(nullptr, newM * v)
					};
					
				}
			};


			m_OutlinePixelConstantBuffer = ConstantBuffer::Resolve(Shader::PIXEL_SHADER, DX::XMFLOAT4(1.f, 0.4f, 0.4f, 1.0f), 0, outlineTag);

			draw.AddBindable(m_OutlineVS);
			draw.AddBindable(m_OutlinePS);
			draw.AddBindable(m_OutlineVertexBuffer);
			draw.AddBindable(m_OutlineIndexBuffer);
			draw.SetIndexCount(m_OutlineIndexBuffer->GetCount());
			draw.AddBindable(std::make_shared<TransformCBuffScaling>());
			draw.AddBindable(m_OutlinePixelConstantBuffer);

			outline.AddStep(std::move(draw));
		}
		AddTechnique(std::move(outline));
	}
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

	//Renderer::UpdateConstantBuffer(m_TransformConstantBuffer, cb);
	//Renderer::Bind({ m_VertexShader, m_PixelShader }, m_VertexBuffer, m_IndexBuffer, { m_Texture }, { m_TransformConstantBuffer, m_PixelConstantBuffer }, m_Blender, m_DepthStencil);
	Renderer::Draw();

}

void Cube::DrawOutline()
{
	auto outlineTransform = DX::XMMatrixTranspose(DX::XMMatrixScaling(1.03f, 1.03f, 1.03f) * m_Transform * m_ViewMatrix * m_ProjectionMatrix);
	
	Renderer::UpdateConstantBuffer(m_OutlineTransformConstantBuffer, outlineTransform);
	Renderer::Bind({ m_OutlineVS, m_OutlinePS }, m_OutlineVertexBuffer, m_OutlineIndexBuffer, {}, { m_OutlineTransformConstantBuffer, m_OutlinePixelConstantBuffer}, m_Blender, m_OutlineDepthStencil);
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
