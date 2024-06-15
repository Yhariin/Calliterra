#pragma once
#include <d3d11.h>
#include <wrl.h>

#include "Core/Window.h"
#include "Renderer/GraphicsContext.h"

using Microsoft::WRL::ComPtr;

struct DX11ContextProps
{
	bool Enabled4xMSAA = false;
};

class DX11Context : public GraphicsContext
{
public:
	DX11Context(HWND* hWnd, WindowProps& windowProps);

	virtual void Init() override;
	virtual void SwapBuffers() override;
	virtual void OnWindowResize() override;
private:
	void ClearBuffer(float r, float g, float b);
	void CreateDeviceContext();
	void CreateSwapChain();
	void CreateRenderTargetView();
	void CreateDepthStencilBuffer();
	void SetRenderViewport(float x, float y, float width, float height);
private:
	HWND* m_Hwnd;
	WindowProps& m_WindowProps;
	ComPtr<ID3D11Device> m_Device;
	ComPtr<IDXGISwapChain> m_SwapChain;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;

	ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
	ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_DepthStencilView;


	UINT m_4xMSAAQuality;
	DX11ContextProps m_DX11ContextProps;
};

