#pragma once
#include "Bindable.h"
#include "Shader.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Blender.h"
#include "DepthStencil.h"

// Keeps a unordered map containing bindables. Calling resolve will create a new
// bindable and store it in the map if it's not already inside. If it already exists
// then a shared_ptr will be returned instead. Prevents duplication of renderer resources.
class RendererResourceLibrary
{
public:
	RendererResourceLibrary() = default;
	~RendererResourceLibrary() = default;

	// Type is the type of bindable we are trying to create, Params will be the 
	// required parameters needed to create the bindable.
	template<typename Type, typename...Params>
	static std::shared_ptr<Type> Resolve(Params&...p)
	{
		const auto key = Type::GenerateUID(p...);
		const auto i = m_Bindables.find(key);

		// The bindable is not inside our map, so we need to create it and store it
		if (i == m_Bindables.end())
		{
			std::shared_ptr<Type> bind;

			// We perform a compile-time check of the type of bindable we are trying
			// to create and call the appropriate creation function.
			if constexpr (std::is_same<Type, Shader>::value)
			{
				bind = Renderer::CreateShader(p...);
			}
			else if constexpr(std::is_same<Type, Texture>::value)
			{
				bind = Renderer::CreateTexture(p...);
			}
			else if constexpr(std::is_same<Type, VertexBuffer>::value)
			{
				// VertexBuffer uses a user provided tag for hashing, which occupies the first parameter.
				// Since CreateVertexBuffer() doesn't need this tag we'll use this std::tie trick to
				// get the other parameters we need.
				const auto& vertices = std::get<1>(std::tie(p...));
				const auto& shader = std::get<2>(std::tie(p...));
				bind = Renderer::CreateVertexBuffer(vertices, shader);
			}
			else if constexpr(std::is_same<Type, IndexBuffer>::value)
			{
				const auto& indices = std::get<1>(std::tie(p...));
				bind = Renderer::CreateIndexBuffer(indices);
			}
			else if constexpr(std::is_same<Type, ConstantBuffer>::value)
			{
				// ConstantBuffer uses a user provided tag for hashing, which occupies the first parameter.
				// Since CreateConstantBuffer() doesn't need this tag we'll use this std::tie trick to
				// get the other parameters we need.
				const auto& shaderType = std::get<1>(std::tie(p...));
				const auto& constants = std::get<2>(std::tie(p...));
				const auto& slot = std::get<3>(std::tie(p...));
				bind = Renderer::CreateConstantBuffer(shaderType, constants, slot);
			}
			else if constexpr (std::is_same<Type, Blender>::value)
			{
				bind = Renderer::CreateBlendState(p...);
			}
			else if constexpr (std::is_same<Type, DepthStencil>::value)
			{
				bind = Renderer::CreateDepthStencilState(p...);
			}

			m_Bindables[key] = bind;
			return bind;
		}
		// The bindable is in our map, so we can return it
		else 
		{
			// Since we are storing map values as Bindables we need to use this cast to return them as the intended type
			return std::static_pointer_cast<Type>(i->second);
		}
	}

	std::unordered_map<std::string, std::shared_ptr<Bindable>> GetBindables() { return m_Bindables; }

private:
	inline static std::unordered_map<std::string, std::shared_ptr<Bindable>> m_Bindables;
};

