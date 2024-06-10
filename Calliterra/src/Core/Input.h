#pragma once
#include "KeyCodes.h"

class Input
{
	friend class Window;

public:
	static bool IsKeyPressed(KeyCode key);
	static bool IsMouseButtonPressed(KeyCode key);

	static int GetMouseX();
	static int GetMouseY();
	static std::pair<int, int> GetMousePos();
public:
	static constexpr unsigned int NUM_KEY_CODES = 256;

private:
	static void OnKeyPressed(KeyCode key);
	static void OnKeyReleased(KeyCode key);

	static void OnMouseMove(int x, int y);
	static void OnMouseButtonPressed(KeyCode key);
	static void OnMouseButtonReleased(KeyCode key);
private:
	static std::bitset<NUM_KEY_CODES> m_KeyStates;
	static int m_MouseX;
	static int m_MouseY;
	static int m_WheelDeltaCarry;
	static uint32_t m_KeyRepeatCount[Input::NUM_KEY_CODES];

};

