#pragma once
#include "Renderer/Drawable.h"

class Cube : public Drawable
{
public:
	Cube(DX::XMMATRIX transform = DX::XMMatrixIdentity(), DX::XMFLOAT3 color = {-1.f, -1.f, -1.f});
    
	void InitBuffers();

	void Update(float dt) override;
	void LinkTechniques() override;

private:
	static void CalculateNormals();

private:
	inline static constexpr uint32_t m_VERTEXCOUNT= 8 * 3 * 3;
	inline static constexpr uint32_t m_INDEXCOUNT = 36;

	struct CubeVertex
	{
		DX::XMFLOAT3 Position;
		DX::XMFLOAT3 Normal;
		DX::XMFLOAT2 Texture;
	};

	struct CubePixelConstantBuffer
	{
		float SpecularIntensity;
		float SpecularPower;
		float padding[2];
	};

	inline static constexpr float side = 1.0f / 2.f;
	inline static const std::vector<float> m_CubeVertices =
	{
		-side, -side, -side, 
		 side, -side, -side, 
		-side,  side, -side, 
		 side,  side, -side, 
		-side, -side,  side, 
		 side, -side,  side, 
		-side,  side,  side, 
		 side,  side,  side 
	};

	inline static std::vector<CubeVertex> m_IndependentCubeVertices =
	{
		{{ -side, -side, -side }, { 0.f, 0.f, 0.f }, {0.f, 0.f}},
		{{  side, -side, -side }, { 0.f, 0.f, 0.f }, {1.f, 0.f}},
		{{ -side,  side, -side }, { 0.f, 0.f, 0.f }, {0.f, 1.f}},
		{{  side,  side, -side }, { 0.f, 0.f, 0.f }, {1.f, 1.f}},
		{{ -side, -side,  side }, { 0.f, 0.f, 0.f }, {0.f, 0.f}},
		{{  side, -side,  side }, { 0.f, 0.f, 0.f }, {1.f, 0.f}},
		{{ -side,  side,  side }, { 0.f, 0.f, 0.f }, {0.f, 1.f}},
		{{  side,  side,  side }, { 0.f, 0.f, 0.f }, {1.f, 1.f}},
		{{ -side, -side, -side }, { 0.f, 0.f, 0.f }, {0.f, 0.f}},
		{{ -side,  side, -side }, { 0.f, 0.f, 0.f }, {1.f, 0.f}},
		{{ -side, -side,  side }, { 0.f, 0.f, 0.f }, {0.f, 1.f}},
		{{ -side,  side,  side }, { 0.f, 0.f, 0.f }, {1.f, 1.f}},
		{{  side, -side, -side }, { 0.f, 0.f, 0.f }, {0.f, 0.f}},
		{{  side,  side, -side }, { 0.f, 0.f, 0.f }, {1.f, 0.f}},
		{{  side, -side,  side }, { 0.f, 0.f, 0.f }, {0.f, 1.f}},
		{{  side,  side,  side }, { 0.f, 0.f, 0.f }, {1.f, 1.f}},
		{{ -side, -side, -side }, { 0.f, 0.f, 0.f }, {0.f, 0.f}},
		{{  side, -side, -side }, { 0.f, 0.f, 0.f }, {1.f, 0.f}},
		{{ -side, -side,  side }, { 0.f, 0.f, 0.f }, {0.f, 1.f}},
		{{  side, -side,  side }, { 0.f, 0.f, 0.f }, {1.f, 1.f}},
		{{ -side,  side, -side }, { 0.f, 0.f, 0.f }, {0.f, 0.f}},
		{{  side,  side, -side }, { 0.f, 0.f, 0.f }, {1.f, 0.f}},
		{{ -side,  side,  side }, { 0.f, 0.f, 0.f }, {0.f, 1.f}},
		{{  side,  side,  side }, { 0.f, 0.f, 0.f }, {1.f, 1.f}},

	};

	inline static std::vector<uint32_t> m_CubeIndices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	inline static std::vector<uint32_t> m_IndependentCubeIndices =
	{
		 0, 2, 1,  2, 3, 1,
		 4, 5, 7,  4, 7, 6,
		 8,10, 9, 10,11, 9,
		12,13,15, 12,15,14,
		16,17,18, 18,17,19,
		20,23,21, 20,22,23
	};

};

