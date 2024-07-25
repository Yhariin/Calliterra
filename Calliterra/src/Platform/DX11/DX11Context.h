#pragma once
#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include "Core/Window.h"
#include "Renderer/GraphicsContext.h"
#include "Core/GlobalSettings.h"

using Microsoft::WRL::ComPtr;

struct DX11ContextProps
{
	bool Enabled4xMSAA = false;
	D3D11_FILL_MODE FillMode = D3D11_FILL_SOLID;
	D3D11_CULL_MODE CullMode = D3D11_CULL_BACK;
	
};

class DX11Context : public GraphicsContext, public SettingsSubscriber
{
public:
	DX11Context(HWND* hWnd, WindowProps& windowProps);

	void Init() override;
	void SwapBuffers() override;
	void OnWindowResize() override;
	void Clear() override;
	void SetClearColor(float r, float g, float b, float a) override;

	void OnSettingsUpdate(SettingsType type) override;

	void ToggleWireFrame() override;

	void DrawIndexed(uint32_t indexCount);

	ID3D11Device& GetDevice() const { return *m_Device.Get(); }
	ID3D11DeviceContext& GetDeviceContext() const { return *m_DeviceContext.Get(); }

private:
	void ClearBuffer(float r, float g, float b);
	void CreateDeviceContext();
	void CreateSwapChain();
	void CreateRenderTargetView();
	void CreateDepthStencilBuffer();
	void CreateRasterizerState();
	void SetRenderViewport(float x, float y, float width, float height);
private:
	HWND* m_Hwnd;
	WindowProps& m_WindowProps;
	ComPtr<ID3D11Device> m_Device;
	ComPtr<IDXGISwapChain1> m_SwapChain;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;

	ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
	ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	ComPtr<ID3D11RasterizerState> m_RasterizerState;
	ComPtr<ID3D11DepthStencilState> m_DepthStencilState;

	float m_BufferClearColor[4] = { 1.f, 1.f, 1.f, 1.f };
	UINT m_4xMSAAQuality = 0;
	bool m_VSyncEnabled = true;
	DX11ContextProps m_DX11ContextProps;
};

