#include "pch.h"

#include "Core/Timer.h"
#include "DX11Context.h"
#include <dxgi1_2.h>

#include <backends/imgui_impl_dx11.cpp>

DX11Context::DX11Context(HWND* hWnd, WindowProps& windowProps)
	: m_Hwnd(hWnd), m_WindowProps(windowProps)
{
	ASSERT(hWnd, "Window handle is null!");
}

void DX11Context::Init()
{
	CreateDeviceContext();
	CreateSwapChain();
	CreateRenderTargetView();
	CreateDepthStencilBuffer();
	CreateRasterizerState();
	SetRenderViewport(0.0f, 0.0f, static_cast<float>(m_WindowProps.Width), static_cast<float>(m_WindowProps.Height));

	m_OutputDevices = DX11OutputDevices(m_Device);

	ImGui_ImplDX11_Init(m_Device.Get(), m_DeviceContext.Get());

	std::vector<SettingsType> settings = { SettingsType::IsWireFrame, SettingsType::CullMode };
	GlobalSettings::Register(settings, this);
}

void DX11Context::SwapBuffers()
{

	ASSERT_HR(m_SwapChain->Present(m_VSyncEnabled, 0));

}

void DX11Context::Clear()
{
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), m_BufferClearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void DX11Context::SetClearColor(float r, float g, float b, float a)
{
	m_BufferClearColor[0] = r;
	m_BufferClearColor[1] = g;
	m_BufferClearColor[2] = b;
	m_BufferClearColor[3] = a;
}

void DX11Context::OnSettingsUpdate(SettingsType type)
{
	switch (type)
	{
	case SettingsType::IsWireFrame:
	{
		if (GlobalSettings::Rendering::IsWireFrame())
		{
			m_DX11ContextProps.FillMode = D3D11_FILL_WIREFRAME;
			CreateRasterizerState();
		}
		else
		{
			m_DX11ContextProps.FillMode = D3D11_FILL_SOLID;
			CreateRasterizerState();
		}
		break;
	}
	case SettingsType::CullMode:
	{
		switch (GlobalSettings::Rendering::CullType())
		{
		case GlobalSettings::Rendering::CullNone:
		{
			m_DX11ContextProps.CullMode = D3D11_CULL_NONE;
			CreateRasterizerState();
			break;
		}
		case GlobalSettings::Rendering::CullFront:
		{
			m_DX11ContextProps.CullMode = D3D11_CULL_FRONT;
			CreateRasterizerState();
			break;
		}
		case GlobalSettings::Rendering::CullBack:
		{
			m_DX11ContextProps.CullMode = D3D11_CULL_BACK;
			CreateRasterizerState();
			break;
		}
		}
		break;
	}
	}
}

void DX11Context::DrawIndexed(uint32_t indexCount)
{
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->RSSetState(m_RasterizerState.Get());
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 0);

	m_DeviceContext->DrawIndexed(indexCount, 0, 0);
}

void DX11Context::CreateDeviceContext()
{
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL featureLevelArray[] = { D3D_FEATURE_LEVEL_11_1 };
	UINT feautreLevelCount = sizeof(featureLevelArray) / sizeof(D3D_FEATURE_LEVEL);

	UINT createDeviceFlags= 0;
#ifdef DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	ASSERT_HR(
		D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createDeviceFlags,
			featureLevelArray,
			feautreLevelCount,
			D3D11_SDK_VERSION,
			&m_Device,
			&featureLevel,
			&m_DeviceContext
		)
	);

	if(featureLevel == D3D_FEATURE_LEVEL_11_1)
		LOG_INFO("DirectX 11.1 Context Initialized");

#ifdef DEBUG
	// Set up debug layer to break on D3D11 errors
	ComPtr<ID3D11Debug> d3dDebug;
	ASSERT_HR(
		m_Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3dDebug)
	);

	ComPtr<ID3D11InfoQueue> d3dInfoQueue;
	ASSERT_HR(
		d3dDebug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue)
	);

	d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
	d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
	d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, true);
#endif

}

void DX11Context::CreateSwapChain()
{
	// NOTE: If we want to change the multisampling settings at runtime,
	// we would have to destroy and recreate the swap chain.
	// https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-flush	
	
	// Since MSAA is not supported when the DXGI flip model we will keep it turned off for now
	// TODO: Implement MSAA support elsewhere
	m_DX11ContextProps.Enabled4xMSAA = false;
	/*
	m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMSAAQuality);
	ASSERT(m_4xMSAAQuality > 0, "4X MSAA not supported!");
	*/

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = 0;
	swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;
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
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGIDevice2> dxgiDevice;
	ASSERT_HR(m_Device->QueryInterface(__uuidof(IDXGIDevice2), (void**)&dxgiDevice));

	ComPtr<IDXGIAdapter> dxgiAdapter;
	ASSERT_HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	ComPtr<IDXGIFactory2> dxgiFactory;
	ASSERT_HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory));

	ASSERT_HR(dxgiFactory->CreateSwapChainForHwnd(
		m_Device.Get(),
		*m_Hwnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&m_SwapChain
		)
	);

	ASSERT_HR(dxgiFactory->MakeWindowAssociation(
		*m_Hwnd,
		DXGI_MWA_NO_ALT_ENTER
		)
	);

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

	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = {};
	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	ASSERT_HR(m_Device->CreateDepthStencilState(&depthStencilStateDesc, &m_DepthStencilState));
}

void DX11Context::ClearBuffer(float r, float g, float b)
{
	const float color[] = { r, g, b, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
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
	ASSERT(m_DeviceContext);
	ASSERT(m_Device);
	ASSERT(m_SwapChain);

	m_RenderTargetView.Reset();
	m_DepthStencilView.Reset();
	m_DepthStencilBuffer.Reset();

	m_SwapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	CreateRenderTargetView();
	CreateDepthStencilBuffer();

	SetRenderViewport(0.0f, 0.0f, static_cast<float>(m_WindowProps.Width), static_cast<float>(m_WindowProps.Height));
}

void DX11Context::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = m_DX11ContextProps.FillMode;
	rasterizerDesc.CullMode = m_DX11ContextProps.CullMode;

	ASSERT_HR(m_Device->CreateRasterizerState(&rasterizerDesc, &m_RasterizerState));
}

void DX11Context::ToggleFullscreen()
{
	BOOL fullscreen;
	ASSERT_HR(m_SwapChain->GetFullscreenState(&fullscreen, nullptr));

	if (fullscreen)
	{
		ASSERT_HR(m_SwapChain->SetFullscreenState(FALSE, nullptr));
	}
	else
	{

		DX11OutputDevice monitor = m_OutputDevices.GetCurrentWindowOutputDevice(*m_Hwnd);
		ComPtr<IDXGIOutput> device = monitor.GetDevice();

		DXGI_MODE_DESC modeDesc = {};
		RECT desktopArea = monitor.GetDesktopArea();

		modeDesc.Width = desktopArea.right - desktopArea.left;
		modeDesc.Height = desktopArea.bottom - desktopArea.top;

		ASSERT_HR(m_SwapChain->ResizeTarget(&modeDesc));
	
		ASSERT_HR(m_SwapChain->SetFullscreenState(TRUE, device.Get()));

		ASSERT_HR(m_SwapChain->ResizeTarget(&modeDesc));

		m_RenderTargetView.Reset();
		m_DepthStencilView.Reset();
		m_DepthStencilBuffer.Reset();
		ASSERT_HR(m_SwapChain->ResizeBuffers(
			0, // 0 preserves the existing number of buffers
			0,
			0,
			DXGI_FORMAT_UNKNOWN, // Preserves the existing format
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
			)
		);

		CreateRenderTargetView();
		CreateDepthStencilBuffer();

		SetRenderViewport(0.0f, 0.0f, static_cast<float>(m_WindowProps.Width), static_cast<float>(m_WindowProps.Height));
	}
}

void DX11Context::ToggleWireFrame()
{
	if (m_DX11ContextProps.FillMode == D3D11_FILL_SOLID)
	{
		m_DX11ContextProps.FillMode = D3D11_FILL_WIREFRAME;
		CreateRasterizerState();
	}
	else
	{
		m_DX11ContextProps.FillMode = D3D11_FILL_SOLID;
		CreateRasterizerState();
	}

}
