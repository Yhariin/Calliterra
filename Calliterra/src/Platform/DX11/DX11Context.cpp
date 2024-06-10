#include "pch.h"
#include "DX11Context.h"

DX11Context::DX11Context(HWND* hWnd)
	: m_Hwnd(hWnd)
{
	//TODO: Add assert to check window handle
}

void DX11Context::Init()
{
	// Create DX11 device context
	LOG_INFO("DX11 Context Initialized");
}

void DX11Context::SwapBuffers()
{

}
