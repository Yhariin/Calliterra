#pragma once

#include "RendererAPI.h"
#include "RendererResourceLibrary.h"
#include "RenderQueue/RenderQueue.h"
#include "GraphicsContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Blender.h"
#include "DepthStencilMask.h"
#include "Topology.h"
#include "Platform/DX11/DX11ConstantBuffer.h"
#include "Platform/DX11/DX11VertexBuffer.h"

class RenderGraph;

class Renderer
{
public:
	static void Init(std::shared_ptr<GraphicsContext> graphicsContext);
	static void Shutdown();
	static void Bind(const std::vector<std::shared_ptr<Shader>>& shaderList = {},
					 const std::shared_ptr<VertexBuffer>& vertexBuffer = nullptr,
					 const std::shared_ptr<IndexBuffer>& indexBuffer = nullptr,
					 const std::vector<std::shared_ptr<Texture>>& textures = {},
					 const std::vector<std::shared_ptr<ConstantBuffer>>& constantBufferList = {},
					 const std::shared_ptr<Blender>& blender = nullptr,
					 const std::shared_ptr<DepthStencilMask>& depthStencil = nullptr
	);
	static void Bind(const std::vector<std::shared_ptr<Bindable>>& bindables, uint32_t indexCount);
	static void Draw();

	template<typename Type>
	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(const std::vector<Type>& vertices)
	{
		switch(GetAPI())
		{
		case RendererAPI::None: 
			ASSERT(false, "RendererAPI is set to None!");
			return nullptr;
		case RendererAPI::DX11:
			return std::make_shared<DX11VertexBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), vertices);
		}

		LOG_ERROR("Unknown RendererAPI");
		return nullptr;
	}

	template<typename Type>
	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(const std::vector<std::vector<Type>>& listOfVertexArrays)
	{
		switch(GetAPI())
		{
		case RendererAPI::None: 
			ASSERT(false, "RendererAPI is set to None!");
			return nullptr;
		case RendererAPI::DX11:
			return std::make_shared<DX11VertexBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), listOfVertexArrays);
		}

		LOG_ERROR("Unknown RendererAPI");
		return nullptr;
	}

	static std::shared_ptr<IndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& indices);
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
				return std::make_shared<DX11VertexConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), shaderType, constants, slot);
			case Shader::PIXEL_SHADER:
				return std::make_shared<DX11PixelConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), shaderType, constants, slot);
			case Shader::COMPUTE_SHADER:
				return std::make_shared<DX11ComputeConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), shaderType, constants, slot);
			case Shader::GEOMETRY_SHADER:
				return std::make_shared<DX11GeometryConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), shaderType, constants, slot);
			case Shader::HULL_SHADER:
				return std::make_shared<DX11HullConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), shaderType, constants, slot);
			case Shader::DOMAIN_SHADER:
				return std::make_shared<DX11DomainConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), shaderType, constants, slot);
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
				return std::make_shared<DX11VertexConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), shaderType, slot);
			case Shader::PIXEL_SHADER: 
				return std::make_shared<DX11PixelConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), shaderType, slot);
			case Shader::COMPUTE_SHADER:
				return std::make_shared<DX11ComputeConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), shaderType, slot);
			case Shader::GEOMETRY_SHADER:
				return std::make_shared<DX11GeometryConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), shaderType, slot);
			case Shader::HULL_SHADER:
				return std::make_shared<DX11HullConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), shaderType, slot);
			case Shader::DOMAIN_SHADER:
				return std::make_shared<DX11DomainConstantBuffer<Type>>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), shaderType, slot);
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

	static std::shared_ptr<Texture> CreateTexture(const std::string& filepath, uint32_t slot = 0, Texture::Filter filter = Texture::Filter::Anisotropic);
	static std::shared_ptr<Blender> CreateBlendState(bool enableBlending, Blender::BlendFunc srcBlend, Blender::BlendFunc destBlend, Blender::BlendOp blendOp);
	static std::shared_ptr<DepthStencilMask> CreateDepthStencilMask(DepthStencilMask::Mode mode);
	static std::shared_ptr<Topology> CreateTopology(PrimitiveTopology primitiveTopology);

	static std::shared_ptr<ShaderInputRenderTarget> CreateRenderTarget(uint32_t width = 0, uint32_t height = 0, uint32_t slot = 0);
	static std::shared_ptr<DepthStencilBuffer> CreateDepthStencilBuffer(uint32_t width = 0, uint32_t height = 0, bool canBindShaderInput = false);

	static RendererResourceLibrary& GetResourceLibrary();
	static RenderQueue& GetRenderQueue();

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
	inline static std::shared_ptr<RendererAPI> s_RendererAPI = RendererAPI::Create();
	inline static std::shared_ptr<GraphicsContext> s_GraphicsContext = nullptr;
	inline static RendererResourceLibrary s_ResourceLibrary;
	inline static std::unique_ptr<RenderQueue> s_RenderQueue = nullptr;
	inline static uint32_t s_IndexCount = 0;
};

