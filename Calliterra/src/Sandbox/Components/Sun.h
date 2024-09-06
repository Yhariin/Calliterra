#pragma once
#include "Sandbox/BasicShapes/DirectionalLight.h"

class Sun : public DirectionalLight
{
public:
	Sun();

	void Update(float dt) override;
};

