#pragma once

class Material
{
public:
	enum MapTypes
	{
		None = -1,
		Diffuse,
		Specular,
		Normal,
		Albedo,
		NumSupportedMaps,
	};

public:
	Material() = default;

	void AddMaterialMap(MapTypes mapType, const std::string& filepath) { m_MaterialMaps[mapType] = filepath; }
	const std::string& GetMaterialMap(MapTypes mapType) const { return m_MaterialMaps[mapType]; }
	bool HasMaterialMap(MapTypes mapType) const { return m_MaterialMaps[mapType] != ""; }

	std::array<std::string, NumSupportedMaps> GetMaterialMaps() const { return m_MaterialMaps; }

private:
	std::array<std::string, NumSupportedMaps> m_MaterialMaps;
};

