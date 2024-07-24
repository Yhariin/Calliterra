#pragma once
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"

class Camera
{
public:
	Camera(float aspectRatio, float fov);

	void OnUpdate(float dt);
	void OnEvent(Event& e);

	void OnResize(float width, float height);

	void SetProjection(float zNear, float zFar);

	const DX::XMMATRIX& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	const DX::XMMATRIX& GetViewMatrix() const { return m_ViewMatrix; }
	const DX::XMMATRIX& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
private:
	bool OnWindowResize(WindowResizeEvent e);
	void Translate(DX::XMFLOAT3 translation);
	void Rotate(float dx, float dy);
	void RecalculateViewProjectionMatrix();
	void ComputeViewMatrix();

private:
	DX::XMMATRIX m_ProjectionMatrix;
	DX::XMMATRIX m_ViewMatrix;
	DX::XMMATRIX m_ViewProjectionMatrix;

	DX::XMFLOAT3 m_Position = { 0.f, 0.f, 0.f };
	DX::XMFLOAT3 m_LookAt = { 0.f, 0.f, 1.f };
	DX::XMFLOAT3 m_WorldUp = { 0.f, 1.f, 0.f };

	float m_AspectRatio;
	float m_Fov = 90.f;
	float m_Pitch = 0;
	float m_Yaw = 0;

	static constexpr float m_MoveSpeed = 10.f;
	static constexpr float m_Sensitivity = 0.001f;
};

