#pragma once

#include "RendererAPI.h"

class Renderer
{
public:
	static void Init();
	static void Shutdown();

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
private:
	inline static std::unique_ptr<RendererAPI> s_RendererAPI = RendererAPI::Create();
};

