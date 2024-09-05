#include "pch.h"
#include "Cube.h"
#include "Renderer/RenderQueue/Passes/Base/RenderPass.h"

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
		Technique standardTech;
		{
			Step onlyStep(PassName::Lambertian);

			auto vShader = Shader::Resolve("assets/shaders/BPhongTexVS.hlsl", Shader::VERTEX_SHADER);
			onlyStep.AddBindable(Shader::Resolve("assets/shaders/BPhongTexPS.hlsl", Shader::PIXEL_SHADER));
			onlyStep.AddBindable(vShader);
			auto vBuff = VertexBuffer::Resolve(geometryTag, m_IndependentCubeVertices);
			onlyStep.AddBindable(IndexBuffer::Resolve(geometryTag, m_IndependentCubeIndices));

			vBuff->CreateLayout({
				{"POSITION", 0, ShaderDataType::Float3},
				{"NORMAL", 0, ShaderDataType::Float3},
				{"TEXCOORD", 0, ShaderDataType::Float2},
				}, vShader.get());

			onlyStep.AddBindable(vBuff);
			onlyStep.AddBindable(Texture::Resolve("assets/textures/brickwall.jpg"));

			m_TransformConstantBuffer = std::make_shared<TransformConstantBuffer>();

			CubePixelConstantBuffer pcb = {
				0.6f,
				128.f
			};

			onlyStep.AddBindable(ConstantBuffer::Resolve<CubePixelConstantBuffer>(Shader::PIXEL_SHADER, pcb, 1));

			onlyStep.AddBindable(Blender::Resolve(false, Blender::BlendFunc::NONE, Blender::BlendFunc::NONE, Blender::BlendOp::NONE));

			onlyStep.AddBindable(m_TransformConstantBuffer);

			standardTech.AddStep(std::move(onlyStep));
		}
		AddTechnique(std::move(standardTech));
	}
	{
		Technique outlineTech;
		{
			Step maskStep(PassName::OutlineMask);

			auto vShader = Shader::Resolve("assets/shaders/FlatColorVS.hlsl", Shader::VERTEX_SHADER);
			auto pShader = Shader::Resolve("assets/shaders/FlatColorPS.hlsl", Shader::PIXEL_SHADER);
			maskStep.AddBindable(vShader);

			auto vBuff = VertexBuffer::Resolve(outlineTag, m_CubeVertices);
			auto iBuff = IndexBuffer::Resolve(outlineTag, m_CubeIndices);
			vBuff->CreateLayout({
				{"POSITION", 0, ShaderDataType::Float3}
				}, vShader.get());

			maskStep.AddBindable(vBuff);
			maskStep.AddBindable(iBuff);

			maskStep.AddBindable(m_TransformConstantBuffer);

			outlineTech.AddStep(std::move(maskStep));

			//======================================================

			Step drawStep(PassName::OutlineDraw);

			class TransformCBuffScaling : public TransformConstantBuffer
			{
			public:
				using TransformConstantBuffer::TransformConstantBuffer;
				Transforms GetTransforms() const override
				{
					ASSERT(m_Parent != nullptr);
					auto& m = m_Parent->GetTransform();
					auto& v = m_Parent->GetViewTransform();
					auto& p = m_Parent->GetProjectionTransform();

					auto newM = DX::XMMatrixScaling(1.03f, 1.03f, 1.03f) * m;
					return {
						DX::XMMatrixTranspose(newM * v),
						DX::XMMatrixTranspose(newM * v * p),
						DX::XMMatrixInverse(nullptr, newM * v)
					};
					
				}
			};


			drawStep.AddBindable(ConstantBuffer::Resolve(Shader::PIXEL_SHADER, DX::XMFLOAT4(1.f, 0.4f, 0.4f, 1.0f), 0, outlineTag));

			drawStep.AddBindable(vShader);
			drawStep.AddBindable(pShader);
			drawStep.AddBindable(vBuff);
			drawStep.AddBindable(iBuff);
			drawStep.AddBindable(std::make_shared<TransformCBuffScaling>());

			outlineTech.AddStep(std::move(drawStep));
		}
		AddTechnique(std::move(outlineTech));
	}
}

void Cube::Update(float dt)
{
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
