#include "pch.h"
#include "RendererAPI.h"

#include "Platform/DX11/DX11RendererAPI.h"

RendererAPI::API RendererAPI::s_API = DX11;

std::unique_ptr<RendererAPI>RendererAPI::Create()
{
	switch (s_API)
	{
	case None: 
		ASSERT(false, "RendererAPI is set to None!");
		return nullptr;
	case DX11:
		return std::make_unique<DX11RendererAPI>();
	}

	LOG_ERROR("Unknown RendererAPI");
	return nullptr;
	
}
