#include "pch.h"
#include "Camera.h"
#include "Core/Input.h"

static float wrapAngle(float theta)
{
	constexpr float twoPi = static_cast<float>(2 * std::numbers::pi);
	const float mod = (float)fmod(theta, twoPi);
	if (mod > (float)std::numbers::pi)
	{
		return mod - twoPi;
	}
	else if (mod < -(float)std::numbers::pi)
	{
		return mod + twoPi;
	}
	return mod;
}

static void swap(float& a, float& b)
{
	float tmp = a;
	a = b;
	b = tmp;
}

Camera::Camera(float aspectRatio, float fov)
	: m_AspectRatio(aspectRatio), 
	  m_Fov(fov) 
	  //m_ProjectionMatrix(DX::XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.1f, 10.f))
{
	SetProjection(0.1f, 10.f);
	ComputeViewMatrix();
	m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
}

void Camera::OnUpdate(float dt)
{
	if (!Input::IsCursorEnabled())
	{
		if (Input::IsKeyPressed('W'))
		{
			Translate(DX::XMFLOAT3(0.f, 0.f, dt));
			//m_Position.z += m_MoveSpeed * dt;
		}
		if (Input::IsKeyPressed('A'))
		{
			Translate({-dt, 0.f, 0.f});
			//m_Position.x -= m_MoveSpeed * dt;
		}
		if (Input::IsKeyPressed('S'))
		{
			Translate({0.f, 0.f, -dt});
			//m_Position.z -= m_MoveSpeed * dt;
		}
		if (Input::IsKeyPressed('D'))
		{
			Translate({dt, 0.f, 0.f});
			//m_Position.x += m_MoveSpeed * dt;
		}
		if (Input::IsKeyPressed(VK_SPACE))
		{
			Translate({0.f, dt, 0.f});
			//m_Position.y += m_MoveSpeed * dt;
		}
		if (Input::IsKeyPressed(VK_SHIFT))
		{
			Translate({0.f, -dt, 0.f});
			//m_Position.y -= m_MoveSpeed * dt;
		}

		while (const auto delta = Input::ReadRawDelta())
		{
			Rotate((float)delta.value().first, (float)delta.value().second);
		}
		
		RecalculateViewProjectionMatrix();
	}

}

void Camera::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return this->OnWindowResize(e); });
}

void Camera::OnResize(float width, float height)
{
	m_AspectRatio = width / height;
	SetProjection(0.1f, 10.f);
}

void Camera::SetProjection(float zNear, float zFar)
{
	// https://community.khronos.org/t/horizontal-vertical-fov-glm-projection-matrix/73854
	//float f = std::tan(DX::XMConvertToRadians(m_Fov) / 2.f);
	float a = 1.f / m_AspectRatio;
	m_ProjectionMatrix = DX::XMMatrixPerspectiveFovLH(DX::XMConvertToRadians(m_Fov), m_AspectRatio, zNear, zFar);
	//m_ProjectionMatrix = DX::XMMatrixPerspectiveFovLH(DX::XMConvertToRadians(m_Fov), a, zNear, zFar);
	//DX::XMFLOAT4X4 m;
	//DX::XMStoreFloat4x4(&m, m_ProjectionMatrix);
	//swap(m._11, m._22);

	//m_ProjectionMatrix = DX::XMLoadFloat4x4(&m);
}

bool Camera::OnWindowResize(WindowResizeEvent e)
{
	m_AspectRatio = static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight());
	return true;
}

void Camera::ComputeViewMatrix()
{
	const DX::XMVECTOR lookVector = DX::XMVector3Transform(DX::XMLoadFloat3(&m_LookAt), DX::XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, 0.f));

	const DX::XMVECTOR camPos = XMLoadFloat3(&m_Position);
	const DX::XMVECTOR camTarget = DX::XMVectorAdd(camPos, lookVector);

	m_ViewMatrix = DX::XMMatrixLookAtLH(camPos, camTarget, DX::XMVECTOR({ 0.f, 1.f, 0.f, 0.f }));

}

void Camera::RecalculateViewProjectionMatrix()
{
	ComputeViewMatrix();
	m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
}

void Camera::Translate(DX::XMFLOAT3 translation)
{
	DX::XMStoreFloat3(&translation, DX::XMVector3Transform(
		DX::XMLoadFloat3(&translation),
		DX::XMMatrixRotationRollPitchYaw(0.f, m_Yaw, 0.f) * // May need to also do pitch here
		DX::XMMatrixScaling(m_MoveSpeed, m_MoveSpeed, m_MoveSpeed)
	));


	m_Position = {
		m_Position.x + translation.x,
		m_Position.y + translation.y,
		m_Position.z + translation.z
	};
}

void Camera::Rotate(float dx, float dy)
{
	m_Yaw = wrapAngle(m_Yaw + (dx * m_Sensitivity));
	m_Pitch = std::clamp(m_Pitch + (dy * m_Sensitivity), 0.995f * (float)-std::numbers::pi / 2.0f, 0.995f * (float)std::numbers::pi / 2.0f);
}
