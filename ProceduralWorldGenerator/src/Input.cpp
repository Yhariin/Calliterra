#include "pch.h"
#include "Input.h"

// Default bitset constructor sets all bits to 0
std::bitset<Input::NUM_KEY_CODES> Input::m_KeyStates = std::bitset<Input::NUM_KEY_CODES>();
int Input::m_MouseX = 0;
int Input::m_MouseY = 0;

bool Input::IsKeyPressed(KeyCode key)
{
	return m_KeyStates[key];
}

bool Input::IsMouseButtonPressed(KeyCode key)
{
	return true;
}

float Input::GetMouseX()
{
	return m_MouseX;
}

float Input::GetMouseY()
{
	return m_MouseY;
}

std::pair<int, int> Input::GetMousePos()
{
	return { m_MouseX, m_MouseY };
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

void Input::OnMouseButtonPressed(KeyCode key)
{
	m_KeyStates[key] = true;
}

void Input::OnMouseButtonReleased(KeyCode key)
{
	m_KeyStates[key] = false;
}
