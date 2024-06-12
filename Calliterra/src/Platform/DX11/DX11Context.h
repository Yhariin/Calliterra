#pragma once
#include <d3d11.h>

#include "Renderer/GraphicsContext.h"

struct DX11ContextProps
{
	bool Enabled4xMSAA = false;
};

class DX11Context : public GraphicsContext
{
public:
	DX11Context(HWND* hWnd);

	virtual void Init() override;
	virtual void SwapBuffers() override;
private:
	void CreateDeviceContext();
	void CreateSwapChain();
private:
	HWND* m_Hwnd;
	ID3D11Device* m_Device = nullptr;
	IDXGISwapChain* m_SwapChain = nullptr;
	ID3D11DeviceContext* m_DeviceContext = nullptr;
	UINT m_4xMSAAQuality;
	DX11ContextProps m_DX11ContextProps;
};

