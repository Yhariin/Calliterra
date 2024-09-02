#pragma once

class GraphicsContext;
class RenderTarget;
class DepthStencilBuffer;

class Pass;
class RenderQueuePass;
class Source;
class Sink;

class RenderGraph
{
public:
	RenderGraph(const GraphicsContext&);
	~RenderGraph(); // https://ortogonal.github.io/cpp/forward-declaration-and-smart-pointers/ 

	void Execute();
	void Reset();

	RenderQueuePass& GetRenderQueue(const std::string& passName);

private:
	void SetSinkTarget(const std::string& sinkName, const std::string& target);
	void AddGlobalSource(std::unique_ptr<Source> source);
	void AddGlobalSink(std::unique_ptr<Sink> sink);
	void AppendPass(std::unique_ptr<Pass> pass);
	void Finalize();

	void LinkSinks(Pass& pass);
	void LinkGlobalSinks();

private:
	const GraphicsContext& m_Context;
	std::vector<std::unique_ptr<Pass>> m_Passes;
	std::vector<std::unique_ptr<Source>> m_GlobalSources;
	std::vector<std::unique_ptr<Sink>> m_GlobalSinks;
	std::shared_ptr<RenderTarget> m_BackBufferTarget;
	std::shared_ptr<DepthStencilBuffer> m_DepthStencilBuffer;
	bool m_Finalized = false;
};

