#pragma once

struct WindowProps;
class RenderTarget;
class RenderQueue;

class GraphicsContext
{
public:
	virtual ~GraphicsContext() = default;

	virtual void Init() = 0;
	virtual void SwapBuffers() = 0;
	virtual void OnWindowResize() = 0;
	virtual void FreeBuffers() = 0;

	void LinkRenderQueueReference(RenderQueue* rq) { m_RenderQueue = rq; }

	virtual std::shared_ptr<RenderTarget> GetBackBufferTarget() const = 0;

	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;

	virtual void ToggleFullscreen() = 0;

	static std::shared_ptr<GraphicsContext> Create(void* window, WindowProps& windowProps);

protected:
	void FreeRenderQueueBuffers();
	void InitRenderQueueBuffers();

protected:
	RenderQueue* m_RenderQueue;
};

