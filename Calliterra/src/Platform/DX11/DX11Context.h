#pragma once
#include <d3d11.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include "Renderer/GraphicsContext.h"
#include "Core/Window.h"
#include "Core/GlobalSettings.h"
#include "DX11OutputDevices.h"

class DX11OutputOnlyRenderTarget;


class DX11Context : public GraphicsContext
{
public:
	DX11Context(HWND* hWnd, WindowProps& windowProps);

	void Init() override;
	void SwapBuffers() override;
	void OnWindowResize() override;

	std::shared_ptr<RenderTarget> GetBackBufferTarget() const override;

	uint32_t GetWidth() const override;
	uint32_t GetHeight() const override;

	void ToggleFullscreen() override;

	void DrawIndexed(uint32_t indexCount);

	ID3D11Device& GetDevice() const { return *m_Device.Get(); }
	ID3D11DeviceContext& GetDeviceContext() const { return *m_DeviceContext.Get(); }
	ID3D11DepthStencilView& GetDepthStencilView() const { return *m_DepthStencilView.Get(); }

private:
	void CreateDeviceContext();
	void CreateSwapChain();
	void CreateRenderTargetView();
	void CreateDepthStencilBuffer();
	void SetRenderViewport(float x, float y, float width, float height);

private:
	HWND* m_Hwnd;
	WindowProps& m_WindowProps;
	DX11OutputDevices m_OutputDevices;
	ComPtr<ID3D11Device> m_Device;
	ComPtr<IDXGISwapChain1> m_SwapChain;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;

	ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	std::shared_ptr<DX11OutputOnlyRenderTarget> m_RenderTarget = nullptr;

	bool m_VSyncEnabled = true;
};

