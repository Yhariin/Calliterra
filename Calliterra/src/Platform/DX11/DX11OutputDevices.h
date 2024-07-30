#pragma once
#include <dxgi1_2.h>
#include <d3d11.h>

class DX11OutputDevice
{
public:
	DX11OutputDevice() = default;
	DX11OutputDevice(ComPtr<IDXGIOutput> device);

	HMONITOR GetMonitor() { return m_Desc.Monitor; }

	// Returns the max area for a maximized window
	RECT GetWorkArea();

	// Returns the max area for the monitor resolution
	RECT GetDesktopArea();

	ComPtr<IDXGIOutput> GetDevice() { return m_Device; }


private:
	ComPtr<IDXGIOutput> m_Device;
	DXGI_OUTPUT_DESC m_Desc;
};

class DX11OutputDevices
{
public:
	DX11OutputDevices() = default;
	DX11OutputDevices(ComPtr<ID3D11Device> Device);

	DX11OutputDevice& GetCurrentWindowOutputDevice(HWND hWnd);

private:
	std::vector<DX11OutputDevice> m_OutputDevices;
	ComPtr<IDXGIAdapter> m_Adapter;
};

