#include "pch.h"
#include "Input.h"

bool Input::IsKeyPressed(KeyCode key)
{
	return m_KeyStates[key];
}

bool Input::IsMouseButtonPressed(KeyCode key)
{
	return m_KeyStates[key];
}

int Input::GetMouseX()
{
	return m_MouseX;
}

int Input::GetMouseY()
{
	return m_MouseY;
}

std::pair<int, int> Input::GetMousePos()
{
	return { m_MouseX, m_MouseY };
}

void Input::EnableMousePositionInput()
{
	m_IsMousePositionInputEnabled = true;
}

void Input::DisableMousePositionInput()
{
	m_IsMousePositionInputEnabled = false;
}

bool Input::IsMousePositionInputEnabled()
{
	return m_IsMousePositionInputEnabled;
}

std::optional<std::pair<int, int>> Input::ReadRawDelta()
{
	if (m_MouseRawDeltaBuffer.empty())
	{
		return std::nullopt;
	}
	const std::pair<int, int> d = m_MouseRawDeltaBuffer.front();
	m_MouseRawDeltaBuffer.pop();
	return d;
}

void Input::EnableMouseRawInput()
{
	m_IsMouseRawInputEnabled = true;
}

void Input::DisableMouseRawInput()
{
	m_IsMouseRawInputEnabled = false;
}

bool Input::IsMouseRawInputEnabled()
{
	return m_IsMouseRawInputEnabled;
}

void Input::OnKeyPressed(KeyCode key)
{
	m_KeyStates[key] = true;
}

void Input::OnKeyReleased(KeyCode key)
{
	m_KeyStates[key] = false;
}

void Input::OnMouseMove(int x, int y)
{
	m_MouseX = x;
	m_MouseY = y;
}

void Input::OnRawMouseDelta(int dx, int dy)
{
	m_MouseRawDeltaBuffer.push({ dx, dy });
}

void Input::OnMouseButtonPressed(KeyCode key)
{
	m_KeyStates[key] = true;
}

void Input::OnMouseButtonReleased(KeyCode key)
{
	m_KeyStates[key] = false;
}

void Input::OnCursorVisibility(bool isVisible)
{
	m_IsCursorVisible = isVisible;
}
