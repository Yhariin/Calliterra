#pragma once
#include <d3d11.h>

#include "Renderer/GraphicsContext.h"

class DX11Context : public GraphicsContext
{
public:
	DX11Context(HWND* hWnd);

	virtual void Init() override;
	virtual void SwapBuffers() override;
private:
	HWND* m_Hwnd;
	ID3D11Device* m_Device = nullptr;
	IDXGISwapChain* m_Swapchain = nullptr;
	ID3D11DeviceContext* m_DeviceContex = nullptr;
};

