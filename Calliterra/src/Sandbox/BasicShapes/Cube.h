#pragma once
#include "Renderer/Drawable.h"

class Cube : public Drawable
{
public:
	Cube(DX::XMMATRIX transform = DX::XMMatrixIdentity(), DX::XMFLOAT3 color = {-1.f, -1.f, -1.f});
    
	void InitRNG();
	static void InitBuffers();

	void Update(float dt) override;

	void Draw() override;
private:
	static void CalculateNormals();
private:
	// Positional
	float m_R;
	float m_Roll = 0.f;
	float m_Pitch = 0.f;
	float m_Yaw = 0.f;
	float m_Theta;
	float m_Phi;
	float m_Chi;
	// Speed
	float m_dRoll;
	float m_dPitch;
	float m_dYaw;
	float m_dTheta;
	float m_dPhi;
	float m_dChi;

	inline static const uint32_t m_VERTEXCOUNT= 8 * 3 * 3;
	inline static const uint32_t m_INDEXCOUNT = 36;

	struct CubeVertex
	{
		DX::XMFLOAT3 Position;
		DX::XMFLOAT3 Normal;
		DX::XMFLOAT2 Texture;
	};

	struct CubeTransformConstantBuffer
	{
		DX::XMMATRIX Model;
		DX::XMMATRIX ModelView;
		DX::XMMATRIX ModelViewProj;
		DX::XMMATRIX NormalMatrix;
	};

	struct CubePixelConstantBuffer
	{
		alignas(16) DX::XMFLOAT3 Color;
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

	inline static uint32_t m_CubeIndices[m_INDEXCOUNT] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	inline static uint32_t m_IndependentCubeIndices[] =
	{
		 0, 2, 1,  2, 3, 1,
		 4, 5, 7,  4, 7, 6,
		 8,10, 9, 10,11, 9,
		12,13,15, 12,15,14,
		16,17,18, 18,17,19,
		20,23,21, 20,22,23
	};

	inline static std::shared_ptr<Shader> s_VertexShader = nullptr;
	inline static std::shared_ptr<Shader> s_PixelShader = nullptr;
	inline static std::shared_ptr<VertexBuffer> s_VertexBuffer = nullptr;
	inline static std::shared_ptr<IndexBuffer> s_IndexBuffer = nullptr;
	inline static std::shared_ptr<Texture> s_Texture = nullptr;
	inline static std::shared_ptr<ConstantBuffer> s_TransformConstantBuffer = nullptr;
	inline static std::shared_ptr<ConstantBuffer> s_PixelConstantBuffer = nullptr;

};

