#pragma once

#include "RendererAPI.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/ConstantBuffer.h"
#include "Renderer/Texture.h"
#include "Platform/DX11/DX11ConstantBuffer.h"
#include "Platform/DX11/DX11VertexBuffer.h"

class Renderer
{
public:
	static void Init(std::shared_ptr<GraphicsContext> graphicsContext);
	static void Shutdown();
	static void SetClearColor(float r, float g, float b, float a = 1.f);
	static void Clear();
	static void Bind(const std::vector<std::shared_ptr<Shader>>& shaderList = {},
					 const std::shared_ptr<VertexBuffer>& vertexBuffer = nullptr,
					 const std::shared_ptr<IndexBuffer>& indexBuffer = nullptr,
					 const std::shared_ptr<Texture>& texture = nullptr,
					 const std::vector<std::shared_ptr<ConstantBuffer>>& constantBufferList = {}
	);
	static void Draw();

	template<typename Type>
	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(const std::vector<Type>& vertices, uint32_t elementCount, Shader* shader = nullptr)
	{
		switch(GetAPI())
		{
		case RendererAPI::None: 
			ASSERT(false, "RendererAPI is set to None!");
			return nullptr;
		case RendererAPI::DX11:
			ASSERT(shader != nullptr, "Attempting to create DX11VertexBuffer with a Null shader paramater!");
			return std::make_shared<DX11VertexBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), vertices, elementCount, dynamic_cast<DX11Shader*>(shader)->GetCompiledShaderByteCode());
		}

		LOG_ERROR("Unknown RendererAPI");
		return nullptr;
	}

	template<typename Type>
	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(const std::vector<std::vector<Type>>& listOfVertexArrays, const uint32_t* listOfElementCounts, uint32_t bufferCount, Shader* shader = nullptr)
	{
		switch(GetAPI())
		{
		case RendererAPI::None: 
			ASSERT(false, "RendererAPI is set to None!");
			return nullptr;
		case RendererAPI::DX11:
			ASSERT(shader != nullptr, "Attempting to create DX11VertexBuffer with a Null shader paramater!");
			return std::make_shared<DX11VertexBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), listOfVertexArrays, listOfElementCounts, bufferCount, dynamic_cast<DX11Shader*>(shader)->GetCompiledShaderByteCode());
		}

		LOG_ERROR("Unknown RendererAPI");
		return nullptr;
	}
	//static std::shared_ptr<VertexBuffer> CreateVertexBuffer(const float* vertices, uint32_t elementCount, Shader* shader = nullptr);
	//static std::shared_ptr<VertexBuffer> CreateVertexBuffer(const float** listOfVertexArrays, const uint32_t* listOfElementCounts, uint32_t bufferCount, Shader* shader = nullptr);
	static std::shared_ptr<IndexBuffer> CreateIndexBuffer(const uint32_t* indices, uint32_t count);
	static std::shared_ptr<Shader> CreateShader(const std::string& filepath, Shader::ShaderType type);

	template<typename Type>
	static std::shared_ptr<ConstantBuffer> CreateConstantBuffer(Shader::ShaderType shaderType, const Type& constants, uint32_t slot = 0)
	{
		switch (GetAPI())
		{
		case RendererAPI::None:
			ASSERT(false, "RendererAPI is set to None!");
			return nullptr;
		case RendererAPI::DX11:
		{
			switch (shaderType)
			{
			case Shader::UKNOWN:
				ASSERT(false, "Shader type unknown!");
				break;
			case Shader::VERTEX_SHADER:
				return std::make_shared<DX11VertexConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), constants, slot);
			case Shader::PIXEL_SHADER:
				return std::make_shared<DX11PixelConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), constants, slot);
			case Shader::COMPUTE_SHADER:
				return std::make_shared<DX11ComputeConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), constants, slot);
			case Shader::GEOMETRY_SHADER:
				return std::make_shared<DX11GeometryConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), constants, slot);
			case Shader::HULL_SHADER:
				return std::make_shared<DX11HullConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), constants, slot);
			case Shader::DOMAIN_SHADER:
				return std::make_shared<DX11DomainConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), constants, slot);
			}

		}
			
		}

		ASSERT(false, "ConstantBuffer creation is not supported for this API");
		return nullptr;
	}

	template<typename Type>
	static std::shared_ptr<ConstantBuffer> CreateConstantBuffer(Shader::ShaderType shaderType, uint32_t slot = 0)
	{
		switch (GetAPI())
		{
		case RendererAPI::None:
			ASSERT(false, "RendererAPI is set to None!");
			return nullptr;
		case RendererAPI::DX11:
		{
			switch (shaderType)
			{
			case Shader::UKNOWN:
				ASSERT(false, "Shader type unknown!");
				break;
			case Shader::VERTEX_SHADER:
				return std::make_shared<DX11VertexConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), slot);
			case Shader::PIXEL_SHADER:
				return std::make_shared<DX11PixelConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), slot);
			case Shader::COMPUTE_SHADER:
				return std::make_shared<DX11ComputeConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), slot);
			case Shader::GEOMETRY_SHADER:
				return std::make_shared<DX11GeometryConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), slot);
			case Shader::HULL_SHADER:
				return std::make_shared<DX11HullConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), slot);
			case Shader::DOMAIN_SHADER:
				return std::make_shared<DX11DomainConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), slot);
			}
		}

		}
		ASSERT(false, "ConstantBuffer creation is not supported for this API");
		return nullptr;
	}

	// Due to limitations of not being able to have virtual template functions we have the renderer manually
	// type cast and call the appropriate update function for a constant buffer
	template<typename Type>
	static void UpdateConstantBuffer(std::shared_ptr<ConstantBuffer> constantBuffer, Type constants)
	{
		switch (GetAPI())
		{
		case RendererAPI::None:
			ASSERT(false, "RendererAPI is set to None!");
			break;
		case RendererAPI::DX11:
			std::dynamic_pointer_cast<DX11ConstantBuffer<Type>>(constantBuffer)->Update(constants);
			break;
		default:
			ASSERT(false, "ConstantBuffer is not supported for this API");
			break;
		}

	}

	static std::shared_ptr<Texture> CreateTexture(const std::string& filepath, uint32_t slot = 0);

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
private:
	inline static std::shared_ptr<RendererAPI> s_RendererAPI = RendererAPI::Create();
	inline static std::shared_ptr<GraphicsContext> s_GraphicsContext = nullptr;
	inline static uint32_t s_IndexCount = 0;
};

