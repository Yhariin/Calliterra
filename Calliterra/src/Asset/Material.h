#pragma once

class Material
{
public:
	enum MapTypes
	{
		None = -1,
		Albedo,
		Specular,
		Normal,
		NumSupportedMaps,
	};

public:
	Material() = default;

	void AddMaterialMap(MapTypes mapType, const std::string& filepath) { m_MaterialMaps[mapType] = filepath; }
	const std::string& GetMaterialMap(MapTypes mapType) const { return m_MaterialMaps[mapType]; }
	bool HasMaterialMap(MapTypes mapType) const { return m_MaterialMaps[mapType] != ""; }

	void SetShininess(float shine) { m_Shininess = shine; }
	float GetShininess() const { return m_Shininess; }

	std::array<std::string, NumSupportedMaps> GetMaterialMaps() const { return m_MaterialMaps; }

private:
	std::array<std::string, NumSupportedMaps> m_MaterialMaps;
	float m_Shininess;
};

