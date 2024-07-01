#include "pch.h"

#include "Core/Timer.h"
#include "DX11Context.h"
#include <dxgi1_2.h>

DX11Context::DX11Context(HWND* hWnd, WindowProps& windowProps)
	: m_Hwnd(hWnd), m_WindowProps(windowProps), m_4xMSAAQuality(0)
{
	ASSERT(hWnd, "Window handle is null!");
}

void DX11Context::Init()
{
	CreateDeviceContext();
	CreateSwapChain();
	CreateRenderTargetView();
	//CreateDepthStencilBuffer();
	SetRenderViewport(0.0f, 0.0f, static_cast<float>(m_WindowProps.Width), static_cast<float>(m_WindowProps.Height));
}

void DX11Context::SwapBuffers()
{
	const float c = static_cast<float>(sin(Timer::GetApplicationTimer().GetElapsedInSeconds()) / 2.0 + 0.5);
	//ClearBuffer(1, 0, 1);
	ClearBuffer(c, .5, c);
	m_SwapChain->Present(1, 0);
}

void DX11Context::CreateDeviceContext()
{
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL featureLevelArray[] = { D3D_FEATURE_LEVEL_11_1 };
	UINT feautreLevelCount = sizeof(featureLevelArray) / sizeof(D3D_FEATURE_LEVEL);
	
	ASSERT_HR(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		featureLevelArray,
		feautreLevelCount,
		D3D11_SDK_VERSION,
		&m_Device,
		&featureLevel,
		&m_DeviceContext
	));

	if(featureLevel == D3D_FEATURE_LEVEL_11_1)
		LOG_INFO("DirectX 11.1 Context Initialized");

}

void DX11Context::CreateSwapChain()
{
	// NOTE: If we want to change the multisampling settings at runtime,
	// we would have to destroy and recreate the swap chain.
	m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMSAAQuality);
	ASSERT(m_4xMSAAQuality > 0, "4X MSAA not supported!");

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = m_WindowProps.Width;
	swapChainDesc.BufferDesc.Height = m_WindowProps.Height;
	swapChainDesc.BufferDesc.RefreshRate = DXGI_RATIONAL{0, 0};
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	if (m_DX11ContextProps.Enabled4xMSAA)
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m_4xMSAAQuality;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = *m_Hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;


	ComPtr<IDXGIDevice2> dxgiDevice;
	ASSERT_HR(m_Device->QueryInterface(__uuidof(IDXGIDevice2), (void**)&dxgiDevice));

	ComPtr<IDXGIAdapter> dxgiAdapter;
	ASSERT_HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	ComPtr<IDXGIFactory2> dxgiFactory;
	ASSERT_HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory));

	ASSERT_HR(dxgiFactory->CreateSwapChain(
		m_Device.Get(),
		&swapChainDesc,
		&m_SwapChain
	));

}

void DX11Context::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> backBuffer;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);

	m_Device->CreateRenderTargetView(backBuffer.Get(), 0, &m_RenderTargetView);
}

void DX11Context::CreateDepthStencilBuffer()
{
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = m_WindowProps.Width;
	depthStencilDesc.Height = m_WindowProps.Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (m_DX11ContextProps.Enabled4xMSAA)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ASSERT_HR(m_Device->CreateTexture2D(&depthStencilDesc, 0, &m_DepthStencilBuffer));
	ASSERT_HR(m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), 0, &m_DepthStencilView));

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView.Get());
}

void DX11Context::ClearBuffer(float r, float g, float b)
{
	const float color[] = { r, g, b, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
}

void DX11Context::SetRenderViewport(float x, float y, float width, float height)
{
	D3D11_VIEWPORT viewportDesc = {};
	viewportDesc.TopLeftX = x;
	viewportDesc.TopLeftY = y;
	viewportDesc.Width = width;
	viewportDesc.Height = height;
	viewportDesc.MinDepth = 0.0f;
	viewportDesc.MaxDepth = 1.0f;

	m_DeviceContext->RSSetViewports(1, &viewportDesc);
}

void DX11Context::OnWindowResize() 
{
	//LOG_DEBUG("{0}, {1}", m_WindowProps.Width, m_WindowProps.Height);

	ASSERT(m_DeviceContext);
	ASSERT(m_Device);
	ASSERT(m_SwapChain);

	//m_DeviceContext->OMSetRenderTargets(0, 0, 0);
	m_RenderTargetView.Reset();
	m_DepthStencilView.Reset();
	m_DepthStencilBuffer.Reset();

	m_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

	CreateRenderTargetView();
	//CreateDepthStencilBuffer();

	SetRenderViewport(0.0f, 0.0f, static_cast<float>(m_WindowProps.Width), static_cast<float>(m_WindowProps.Height));
}
