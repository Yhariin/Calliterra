#include "pch.h"
#include "IcoSphere.h"

IcoSphere::IcoSphere(const int resolution, DX::XMMATRIX transform, DX::XMFLOAT3 color)
	: Drawable(transform, color)
{
	CalculateSphere(resolution);
	InitBuffers();
}

void IcoSphere::Draw()
{
	Renderer::UpdateConstantBuffer(m_TransformConstantBuffer, DX::XMMatrixTranspose(m_Transform * m_ViewProjectionMatrix));
	Renderer::Bind({ m_VertexShader, m_PixelShader }, m_VertexBuffer, m_IndexBuffer, { m_TransformConstantBuffer, m_ColorConstantBuffer });
	Renderer::Draw();
}

void IcoSphere::Update(float dt)
{

}

void IcoSphere::Update(DX::XMMATRIX transform)
{
	m_Transform = transform;
}

// http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
void IcoSphere::CalculateSphere(const int resolution)
{
	// Create vertices of Icosahedron
	float t = (1.0f + sqrt(5.0f)) / 2.0f;

	AddVertex(DX::XMFLOAT3(-1, t, 0));
	AddVertex(DX::XMFLOAT3(1, t, 0));
	AddVertex(DX::XMFLOAT3(-1, -t, 0));
	AddVertex(DX::XMFLOAT3(1, -t, 0));

	AddVertex(DX::XMFLOAT3(0, -1, t));
	AddVertex(DX::XMFLOAT3(0, 1, t));
	AddVertex(DX::XMFLOAT3(0, -1, -t));
	AddVertex(DX::XMFLOAT3(0, 1, -t));

	AddVertex(DX::XMFLOAT3(t, 0, -1));
	AddVertex(DX::XMFLOAT3(t, 0, 1));
	AddVertex(DX::XMFLOAT3(-t, 0, -1));
	AddVertex(DX::XMFLOAT3(-t, 0, 1));

	// Create the triangle faces of Icosahedron
	std::vector<TriangleIndices> faces =
	{
		{0,11,5}, {0,5,1}, {0,1,7}, {0,7,10}, {0,10,11},
		{1,5,9}, {5,11,4}, {11,10,2}, {10,7,6}, {7,1,8},
		{3,9,4}, {3,4,2}, {3,2,6}, {3,6,8}, {3,8,9},
		{4,9,5}, {2,4,11}, {6,2,10}, {8,6,7}, {9,8,1}
	};

	// Subdivide triangles
	for (int i = 0; i < resolution; i++)
	{
		std::vector<TriangleIndices> faces2;
		for (auto tri : faces)
		{
			int a = GetMidPoint(tri.v1, tri.v2);
			int b = GetMidPoint(tri.v2, tri.v3);
			int c = GetMidPoint(tri.v3, tri.v1);

			faces2.emplace_back(TriangleIndices(tri.v1, a, c));
			faces2.emplace_back(TriangleIndices(tri.v2, b, a));
			faces2.emplace_back(TriangleIndices(tri.v3, c, b));
			faces2.emplace_back(TriangleIndices(a, b, c));
		}

		faces = faces2;
	}

	for(auto tri : faces)
	{
		m_SphereIndices.push_back(tri.v1);
		m_SphereIndices.push_back(tri.v2);
		m_SphereIndices.push_back(tri.v3);
	}
}

int IcoSphere::GetMidPoint(uint32_t p1, uint32_t p2)
{
	bool firstIsSmaller = p1 < p2;
	long long smallerIndex = firstIsSmaller ? p1 : p2;
	long long greaterIndex = firstIsSmaller ? p2 : p1;
	long long key = (smallerIndex << 32) + greaterIndex;

	if (m_MiddlePointCache.contains(key))
	{
		return m_MiddlePointCache.at(key);
	}

	DX::XMFLOAT3 point1 = m_SphereVertices[p1];
	DX::XMFLOAT3 point2 = m_SphereVertices[p2];
	DX::XMFLOAT3 middle = DX::XMFLOAT3(
		(point1.x + point2.x) / 2.f,
		(point1.y + point2.y) / 2.f,
		(point1.z + point2.z) / 2.f
	);


	int i = AddVertex(middle);
	m_MiddlePointCache.insert({ key, i });
	return i;
}

void IcoSphere::InitBuffers()
{
	if(DX::XMVector3Equal(DX::XMLoadFloat3(&m_FlatColor), {-1.f, -1.f, -1.f}))
	{ 
		m_VertexShader = Renderer::CreateShader("assets/shaders/ColorIndexVS.hlsl", Shader::VERTEX_SHADER);
		m_PixelShader = Renderer::CreateShader("assets/shaders/ColorIndexPS.hlsl", Shader::PIXEL_SHADER);

		m_VertexShader->Bind();
		m_PixelShader->Bind();

		m_ColorConstantBuffer = Renderer::CreateConstantBuffer<FaceColorsBuffer>(Shader::PIXEL_SHADER, m_ColorsBuffer);
	}
	else
	{
		m_VertexShader = Renderer::CreateShader("assets/shaders/FlatColorVS.hlsl", Shader::VERTEX_SHADER);
		m_PixelShader = Renderer::CreateShader("assets/shaders/FlatColorPS.hlsl", Shader::PIXEL_SHADER);

		m_VertexShader->Bind();
		m_PixelShader->Bind();

		m_ColorConstantBuffer = Renderer::CreateConstantBuffer<DX::XMFLOAT4>(Shader::PIXEL_SHADER, DX::XMFLOAT4(m_FlatColor.x, m_FlatColor.y, m_FlatColor.z, 1.f));
	}

	m_VertexBuffer = Renderer::CreateVertexBuffer(m_SphereVertices, static_cast<uint32_t>(m_SphereVertices.size()), m_VertexShader.get());
	m_IndexBuffer = Renderer::CreateIndexBuffer(&m_SphereIndices[0], static_cast<uint32_t>(m_SphereIndices.size()));

	m_VertexShader->Bind();
	m_PixelShader->Bind();

	m_VertexBuffer->CreateLayout({
		{"POSITION", 0, ShaderDataType::Float3},
		});
	m_VertexBuffer->SetLayout();

	m_TransformConstantBuffer = Renderer::CreateConstantBuffer<DX::XMMATRIX>(Shader::VERTEX_SHADER);
}

int IcoSphere::AddVertex(DX::XMFLOAT3 point)
{
	float length = sqrt(point.x * point.x + point.y * point.y + point.z * point.z);
	m_SphereVertices.emplace_back(DX::XMFLOAT3(
		point.x / length,
		point.y / length,
		point.z / length));
	return static_cast<int>(m_SphereVertices.size()-1);

}
