#pragma once
#include "Bindable.h"
#include "Renderer/BufferLayout.h"
#include "Shader.h"


class VertexBuffer : public Bindable
{
public:
	virtual ~VertexBuffer() = default;

	virtual void SetLayout(int index = 0) = 0;
	virtual void CreateLayout(const VertexBufferLayout& layout) = 0;
	virtual void CreateLayoutList(const std::vector<VertexBufferLayout>& layoutList) = 0;

	template <typename...IgnoreParams>
	static const std::string GenerateUID(const std::string& tag, IgnoreParams&&...ignore)
	{
		using namespace std::string_literals;
		return typeid(VertexBuffer).name() + "#"s + tag;
	}

	template <typename Type>
	static std::shared_ptr<VertexBuffer> Resolve(const std::string& tag, const std::vector<Type>& vertices, Shader* shader = nullptr)
	{
		return Renderer::GetResourceLibrary().Resolve<VertexBuffer>(tag, vertices, shader);
	}

	template <typename Type>
	static std::shared_ptr<VertexBuffer> Resolve(const std::string& tag, const std::vector<std::vector<Type>>& listOfVertexArrays, uint32_t bufferCount, Shader* shader = nullptr)
	{
		return Renderer::GetResourceLibrary().Resolve<VertexBuffer>(tag, listOfVertexArrays, shader);
	}

};

