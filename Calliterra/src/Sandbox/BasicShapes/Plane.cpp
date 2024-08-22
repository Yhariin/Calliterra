#include "pch.h"
#include "Plane.h"

Plane::Plane(uint32_t resolution, DX::XMMATRIX transform, DX::XMFLOAT3 color )
	: m_Resolution(resolution), Drawable(transform, color)
{
	CalculatePlane(resolution);
	InitBuffers();
}

void Plane::Draw()
{
	TransformConstantBuffer cb = { 
		DX::XMMatrixTranspose(m_Transform * m_ViewMatrix),
		DX::XMMatrixTranspose(m_Transform * m_ViewMatrix * m_ProjectionMatrix),
		DX::XMMatrixInverse(nullptr, m_Transform * m_ViewMatrix)
	};

	Renderer::UpdateConstantBuffer(m_TransformConstantBufferVS, cb);
	Renderer::UpdateConstantBuffer(m_TransformConstantBufferPS, cb);
	Renderer::Bind({ m_VertexShader, m_PixelShader }, m_VertexBuffer, m_IndexBuffer, m_Textures, { m_TransformConstantBufferVS, m_TransformConstantBufferPS, m_PixelConstantBuffer });
	Renderer::Draw();
}

void Plane::Update(float dt)
{

}

void Plane::CalculatePlane(uint32_t resolution)
{
	
	float div = 1.f / resolution;

	for (uint32_t x = 0; x <= resolution; x++)
	{
		for (uint32_t z = 0; z <= resolution; z++)
		{
			m_Vertices.emplace_back(PlaneVertex(DX::XMFLOAT3(x * div, 0.f, z * div), DX::XMFLOAT3(0.f, 0.f, 0.f), {}, {}, DX::XMFLOAT2(z, x)));
		}
	}
	
	const auto calculateIndex = [resolution](auto x, auto z)
		{ return x * (resolution+1) + z; };

	for (uint32_t x = 0; x < resolution; x++)
	{
		for (uint32_t z = 0; z < resolution; z++)
		{
			m_Indices.push_back(calculateIndex(x, z));
			m_Indices.push_back(calculateIndex(x, z+1));
			m_Indices.push_back(calculateIndex(x+1, z));

			m_Indices.push_back(calculateIndex(x, z+1));
			m_Indices.push_back(calculateIndex(x+1, z+1));
			m_Indices.push_back(calculateIndex(x+1, z));

			// Temporary calculate of UVs
			m_Vertices[calculateIndex(x, z)].Texture = DX::XMFLOAT2(1.f, 0.f);
			m_Vertices[calculateIndex(x, z+1)].Texture = DX::XMFLOAT2(1.f, 1.f);
			m_Vertices[calculateIndex(x+1, z)].Texture = DX::XMFLOAT2(0.f, 0.f);
			m_Vertices[calculateIndex(x+1, z+1)].Texture = DX::XMFLOAT2(0.f, 1.f);
		}
	}
	

	CalculateNormals();
	CalculateTangentSpace();

}

void Plane::InitBuffers()
{
	const auto geometryTag = "$Plane." + std::to_string(m_Resolution);

	m_VertexShader = Shader::Resolve("assets/shaders/NormalMapVS.hlsl", Shader::VERTEX_SHADER);
	m_PixelShader = Shader::Resolve("assets/shaders/PhongNormalMapPS.hlsl", Shader::PIXEL_SHADER);
	m_VertexBuffer = VertexBuffer::Resolve(geometryTag, m_Vertices, m_VertexShader.get());
	m_IndexBuffer = IndexBuffer::Resolve(geometryTag, m_Indices);

	m_VertexShader->Bind();
	m_PixelShader->Bind();

	m_VertexBuffer->CreateLayout({
		{"POSITION", 0, ShaderDataType::Float3},
		{"NORMAL", 0, ShaderDataType::Float3},
		{"TANGENT", 0, ShaderDataType::Float3},
		{"BITANGENT", 0, ShaderDataType::Float3},
		{"TEXCOORD", 0, ShaderDataType::Float2},
		});
	m_VertexBuffer->SetLayout();

	m_Textures.emplace_back(Texture::Resolve("assets/textures/brickwall.jpg", 0));
	m_Textures.emplace_back(Texture::Resolve("assets/textures/brickwall_normal.jpg", 1));

	m_TransformConstantBufferVS = ConstantBuffer::Resolve<TransformConstantBuffer>(Shader::VERTEX_SHADER, {}, 0);
	m_TransformConstantBufferPS = ConstantBuffer::Resolve<TransformConstantBuffer>(Shader::PIXEL_SHADER, {}, 0);

	PixelConstantBuffer pcb = {
		.1f,
		20.f
	};
	m_PixelConstantBuffer = ConstantBuffer::Resolve<PixelConstantBuffer>(Shader::PIXEL_SHADER, pcb, 1);

}

void Plane::CalculateNormals()
{
	using namespace DirectX; // For some reason we need to include this line in order to use the XMMath overloaded operators...
	for (int i = 0; i < m_Vertices.size(); i += 3)
	{
		PlaneVertex& v0 = m_Vertices[m_Indices[i]];
		PlaneVertex& v1 = m_Vertices[m_Indices[i + 1]];
		PlaneVertex& v2 = m_Vertices[m_Indices[i + 2]];
		const XMVECTOR p0 = XMLoadFloat3(&v0.Position);
		const XMVECTOR p1 = XMLoadFloat3(&v1.Position);
		const XMVECTOR p2 = XMLoadFloat3(&v2.Position);

		const XMVECTOR normal = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

		XMStoreFloat3(&v0.Normal, normal);
		XMStoreFloat3(&v1.Normal, normal);
		XMStoreFloat3(&v2.Normal, normal);
	}
}

void Plane::CalculateTangentSpace()
{
	ASSERT(m_Indices.size() % 3 == 0);
	for (int i = 0; i < m_Indices.size(); i += 3)
	{
		using namespace DirectX;
		auto& v1 = m_Vertices[m_Indices[i]];
		auto& v2 = m_Vertices[m_Indices[i+1]];
		auto& v3 = m_Vertices[m_Indices[i+2]];

		XMFLOAT3 p1 = v1.Position;
		XMFLOAT3 p2 = v2.Position;
		XMFLOAT3 p3 = v3.Position;

		XMFLOAT2 uv1 = v1.Texture;
		XMFLOAT2 uv2 = v2.Texture;
		XMFLOAT2 uv3 = v3.Texture;

		auto edge1 = XMFLOAT3(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
		auto edge2 = XMFLOAT3(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);
		auto deltaUV1 = XMFLOAT2(uv2.x - uv1.x, uv2.y - uv1.y);
		auto deltaUV2 = XMFLOAT2(uv3.x - uv1.x, uv3.y - uv1.y);

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		XMFLOAT3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		XMFLOAT3 bitangent;
		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		v1.Tangent = tangent;
		v2.Tangent = tangent;
		v3.Tangent = tangent;

		v1.Bitangent = bitangent;
		v2.Bitangent = bitangent;
		v3.Bitangent = bitangent;
	}
}
