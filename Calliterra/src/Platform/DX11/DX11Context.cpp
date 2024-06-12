#include "pch.h"
#include "DX11Context.h"
#include <dxgi1_2.h>

DX11Context::DX11Context(HWND* hWnd)
	: m_Hwnd(hWnd)
{
	ASSERT(hWnd, "Window handle is null!");
}

void DX11Context::Init()
{

	CreateDeviceContext();
	CreateSwapChain();

}

void DX11Context::SwapBuffers()
{

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
	LOG_INFO("{0} Quality", m_4xMSAAQuality);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.RefreshRate = DXGI_RATIONAL{0, 0};
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	if (m_DX11ContextProps.Enabled4xMSAA)
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m_4xMSAAQuality - 1;
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


	IDXGIDevice2* dxgiDevice;
	ASSERT_HR(m_Device->QueryInterface(__uuidof(IDXGIDevice2), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter;
	ASSERT_HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory2* dxgiFactory;
	ASSERT_HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory));

	ASSERT_HR(dxgiFactory->CreateSwapChain(
		m_Device,
		&swapChainDesc,
		&m_SwapChain
	));

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();
}
