#pragma once

struct WindowProps;

class GraphicsContext
{
public:
	virtual ~GraphicsContext() = default;

	virtual void Init() = 0;
	virtual void SwapBuffers() = 0;
	virtual void OnWindowResize() = 0;
	virtual void Clear() = 0;
	virtual void SetClearColor(float r, float g, float b, float a = 1.f) = 0;

	virtual void ToggleWireFrame() = 0;
	static std::shared_ptr<GraphicsContext> Create(void* window, WindowProps& windowProps);
};

