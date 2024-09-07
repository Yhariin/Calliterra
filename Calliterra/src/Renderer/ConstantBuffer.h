#pragma once
#include "Bindable.h"
#include "Shader.h"

class ConstantBuffer : public Bindable
{
public:
	virtual ~ConstantBuffer() = default;

	template <typename...IgnoreParams>
	static const std::string GenerateUID(const std::string& tag, Shader::ShaderType shaderType, IgnoreParams&&...ignore)
	{
		using namespace std::string_literals;
		return typeid(ConstantBuffer).name() + "#"s + tag + "#"s + std::to_string(shaderType);
	}

	template<typename Type>
	static std::shared_ptr<ConstantBuffer> Resolve(Shader::ShaderType shaderType, const Type& constants, uint32_t slot = 0, const std::string& tag = typeid(Type).name())
	{
		return Renderer::GetResourceLibrary().Resolve<ConstantBuffer>(tag, shaderType, constants, slot);
	}

};

class Drawable;

class TransformConstantBuffer : public Bindable
{
public:
	struct Transforms
	{
		DX::XMMATRIX ModelView;
		DX::XMMATRIX ModelViewProj;
		DX::XMMATRIX NormalMatrix;
	};
public:
	TransformConstantBuffer();
	void InitializeParentReference(const Drawable& parent) override;
	virtual Transforms GetTransforms() const;
	void Bind() const override;

protected:
	inline static std::shared_ptr<ConstantBuffer> s_ConstantBuffer = nullptr;
	const Drawable* m_Parent = nullptr;
};

class SkyBoxTransformConstantBuffer : public Bindable
{
public:
	SkyBoxTransformConstantBuffer();
	void InitializeParentReference(const Drawable& parent) override;
	virtual DX::XMMATRIX GetTransform() const;
	void Bind() const override;

protected:
	inline static std::shared_ptr<ConstantBuffer> s_ConstantBuffer = nullptr;
	const Drawable* m_Parent = nullptr;
};
