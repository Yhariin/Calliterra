#pragma once
#include "Sandbox/Drawable.h"

class Cube : public Drawable
{
public:
	Cube();
	Cube(DX::XMMATRIX transform);
    
	void InitRNG();
	static void InitBuffers();

	void Update(DX::XMMATRIX transform) override;
	void Update(float dt) override;
	void Draw() override;
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

	inline static const float m_CubeVertices[m_VERTEXCOUNT] =
	{
		-1.0f, -1.0f, -1.0f, 
		 1.0f, -1.0f, -1.0f, 
		-1.0f,  1.0f, -1.0f, 
		 1.0f,  1.0f, -1.0f, 
		-1.0f, -1.0f,  1.0f, 
		 1.0f, -1.0f,  1.0f, 
		-1.0f,  1.0f,  1.0f, 
		 1.0f,  1.0f,  1.0f 
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

	struct FaceColorsBuffer
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} faceColors[6];
	};

	inline static const FaceColorsBuffer m_ColorsBuffer =
	{
		{
			{1.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 1.0f}

		}
	};

	inline static std::shared_ptr<Shader> s_VertexShader = nullptr;
	inline static std::shared_ptr<Shader> s_PixelShader = nullptr;
	inline static std::shared_ptr<VertexBuffer> s_VertexBuffer = nullptr;
	inline static std::shared_ptr<IndexBuffer> s_IndexBuffer = nullptr;
	inline static std::shared_ptr<ConstantBuffer> s_ConstantBuffer = nullptr;
	inline static std::shared_ptr<ConstantBuffer> s_ConstantBuffer2 = nullptr;

};

