#pragma once

struct WindowProps;

class GraphicsContext
{
public:
	virtual ~GraphicsContext() = default;

	virtual void Init() = 0;
	virtual void SwapBuffers() = 0;
	virtual void OnWindowResize() = 0;

	static std::unique_ptr<GraphicsContext> Create(void* window, WindowProps& windowProps);
};

