#pragma once
#include "Drawable.h"

class Sandbox
{
public:
	Sandbox();

	void OnUpdate(float dt);
	void CreateCube();
private:
	std::vector<std::unique_ptr<Drawable>> m_Drawables;
};
