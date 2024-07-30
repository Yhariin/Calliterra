#include "pch.h"
#include "Platform/DX11/DX11OutputDevices.h"

DX11OutputDevice::DX11OutputDevice(ComPtr<IDXGIOutput> device)
	: m_Device(device)
{
	ASSERT_HR(m_Device->GetDesc(&m_Desc));
}

RECT DX11OutputDevice::GetWorkArea()
{
	MONITORINFOEX info;
	info.cbSize = sizeof(info);

	ASSERT_HR(GetMonitorInfo(m_Desc.Monitor, &info));

	return info.rcWork;
}

RECT DX11OutputDevice::GetDesktopArea()
{
	MONITORINFOEX info;
	info.cbSize = sizeof(info);

	ASSERT_HR(GetMonitorInfo(m_Desc.Monitor, &info));

	return info.rcMonitor;
}

//===========================================================
DX11OutputDevices::DX11OutputDevices(ComPtr<ID3D11Device> device)
{
	ComPtr<IUnknown> deviceUnknown;

	ComPtr<IDXGIDevice> dxgiDevice;
	device->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice);

	ASSERT_HR(dxgiDevice->GetParent(IID_PPV_ARGS(&m_Adapter)));

	UINT index = 0;
	ComPtr<IDXGIOutput> output;

	while (m_Adapter->EnumOutputs(index, &output) != DXGI_ERROR_NOT_FOUND)
	{
		m_OutputDevices.emplace_back(DX11OutputDevice(output));

		index++;
	}
}

DX11OutputDevice& DX11OutputDevices::GetCurrentWindowOutputDevice(HWND hWnd)
{
	HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

	for (auto& device : m_OutputDevices)
	{
		if (device.GetMonitor() == monitor)
		{
			return device;
		}
	}

}
