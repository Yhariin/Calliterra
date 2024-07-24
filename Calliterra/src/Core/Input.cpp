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

int Input::GetMouseDeltaX()
{
	return m_MouseDeltaX;
}

int Input::GetMouseDeltaY()
{
	return m_MouseDeltaY;
}

std::pair<int, int> Input::GetMouseDelta()
{
	return { m_MouseDeltaX, m_MouseDeltaY };
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

void Input::OnMouseDelta(int x, int y)
{
	m_MouseDeltaX = x;
	m_MouseDeltaY = y;
}

void Input::OnMouseButtonPressed(KeyCode key)
{
	m_KeyStates[key] = true;
}

void Input::OnMouseButtonReleased(KeyCode key)
{
	m_KeyStates[key] = false;
}
