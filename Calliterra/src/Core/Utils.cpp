#include "pch.h"
#include "Utils.h"

std::vector<std::string> Utils::SplitString(const std::string& str, const std::string& delim)
{
	std::vector<std::string> strings;
	SplitStringIter(str, delim, std::back_inserter(strings));
	return strings;
}

float Utils::wrapAngle(float theta)
{
	constexpr float twoPi = static_cast<float>(2 * std::numbers::pi);
	const float mod = (float)fmod(theta, twoPi);
	if (mod > std::numbers::pi_v<float>)
	{
		return mod - twoPi;
	}
	else if (mod < -std::numbers::pi_v<float>)
	{
		return mod + twoPi;
	}
	return mod;
}

float Utils::wrapAngleDegrees(float theta)
{
	const float mod = (float)fmod(theta, 360.f);
	if (mod > 360.f)
	{
		return mod - 360.f;
	}
	else if (mod < -360.f)
	{
		return mod + 360.f;
	}
	return mod;
}
