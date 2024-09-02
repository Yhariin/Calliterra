#include "pch.h"
#include "RenderGraph.h"
#include "Passes/Base/Pass.h"
#include "Passes/Base/RenderQueuePass.h"
#include "Renderer/DepthStencilBuffer.h"
#include "Renderer/RenderTarget.h"
#include "Sink.h"
#include "Source.h"
#include "Core/Utils.h"
#include "Renderer/Renderer.h"

#include "Passes/BufferClearPass.h"
#include "Passes/LambertianPass.h"
#include "Passes/OutlineDrawingPass.h"

RenderGraph::RenderGraph(const GraphicsContext& context)
	: m_Context(context), m_BackBufferTarget(context.GetBackBufferTarget()), m_DepthStencilBuffer(Renderer::CreateDepthStencilBuffer(0, 0, false))
{
	AddGlobalSource(DirectBufferSource<RenderTarget>::Make("backBuffer", m_BackBufferTarget));
	AddGlobalSource(DirectBufferSource<DepthStencilBuffer>::Make("masterDepth", m_DepthStencilBuffer));
	AddGlobalSink(DirectBufferSink<RenderTarget>::Make("backBuffer", m_BackBufferTarget));

	{
		auto pass = std::make_unique<BufferClearPass>("clearRT");
		pass->SetSinkLinkage("buffer", "$.backBuffer");
		AppendPass(std::move(pass));
	}
	{
		auto pass = std::make_unique<BufferClearPass>("clearDS");
		pass->SetSinkLinkage("buffer", "$.masterDepth");
		AppendPass(std::move(pass));
	}
	{
		auto pass = std::make_unique<LambertianPass>("lambertian");
		pass->SetSinkLinkage("renderTarget", "clearRT.buffer");
		pass->SetSinkLinkage("depthStencil", "clearDS.buffer");
		AppendPass(std::move(pass));
	}
	//{
	//	auto pass = std::make_unique<OutLine>("lambertian");
	//	pass->SetSinkLinkage("depthStencil", "lambertian.depthStencil");
	//	AppendPass(std::move(pass));
	//}

	Finalize();
}

RenderGraph::~RenderGraph()
{
}

void RenderGraph::Execute()
{
	ASSERT(m_Finalized);
	for (auto& pass : m_Passes)
	{
		pass->Execute();
	}
}

void RenderGraph::Reset()
{
	ASSERT(m_Finalized);
	for (auto& pass : m_Passes)
	{
		pass->Reset();
	}
}

#pragma warning(disable:4715)
RenderQueuePass& RenderGraph::GetRenderQueue(const std::string& passName)
{
	for (const auto& pass : m_Passes)
	{
		if (pass->GetName() == passName)
		{
			return dynamic_cast<RenderQueuePass&>(*pass);
		}
	}

	ASSERT(false, "Pass was not found");
	#pragma warning(default:4715)
}

void RenderGraph::SetSinkTarget(const std::string& sinkName, const std::string& target)
{
	const auto finder = [&sinkName](const std::unique_ptr<Sink>& sink)
		{
			return sink->GetName() == sinkName;
		};

	const auto i = std::find_if(m_GlobalSinks.begin(), m_GlobalSinks.end(), finder);
	ASSERT(i != m_GlobalSinks.end(), "Global sink does not exist");

	auto targetSplit = SplitString(target, ".");
	ASSERT(targetSplit.size() == 2, "Input target has incorrect format");

	(*i)->SetTarget(targetSplit[0], targetSplit[1]);
}

void RenderGraph::AddGlobalSource(std::unique_ptr<Source> source)
{
	m_GlobalSources.push_back(std::move(source));
}

void RenderGraph::AddGlobalSink(std::unique_ptr<Sink> sink)
{
	m_GlobalSinks.push_back(std::move(sink));
}

void RenderGraph::Finalize()
{
	ASSERT(!m_Finalized);

	for (const auto& pass : m_Passes)
	{
		pass->Finalize();
	}
	LinkGlobalSinks();
	m_Finalized = true;
}

void RenderGraph::AppendPass(std::unique_ptr<Pass> pass)
{
	ASSERT(!m_Finalized);

	// Validate name uniqueness in debug builds
	auto validate = [&pass, this]()
		{
			for (const auto& p : m_Passes)
			{
				ASSERT(p->GetName() != pass->GetName(), "Pass name already exists");
			}
		};
	STRIP_DEBUG(validate());

	// Link outputs from passes and global outputs to pass inputs
	LinkSinks(*pass);

	m_Passes.push_back(std::move(pass));
}

void RenderGraph::LinkSinks(Pass& pass)
{
	for (auto& sink : pass.GetSinks())
	{
		const std::string& inputSourcePassName = sink->GetPassName();

		// Check whether target source is global
		if (inputSourcePassName == "$")
		{
			bool bound = false;
			for (auto& source : m_GlobalSources)
			{
				if (source->GetName() == sink->GetSourceName())
				{
					sink->Bind(*source);
					bound = true;
					break;
				}
			}

			ASSERT(bound, "Output name not found in globals");
		}
		else
		{
			for (auto& existingPass : m_Passes)
			{
				if (existingPass->GetName() == inputSourcePassName)
				{
					auto& source = existingPass->GetSource(sink->GetSourceName());
					sink->Bind(source);
					break;
				}
			}

		}
	}
}

void RenderGraph::LinkGlobalSinks()
{
	for (auto& sink : m_GlobalSinks)
	{
		const auto& inputSourcePassName = sink->GetPassName();
		for (auto& existingPass : m_Passes)
		{
			if (existingPass->GetName() == inputSourcePassName)
			{
				auto& source = existingPass->GetSource(sink->GetSourceName());
				sink->Bind(source);
				break;
			}
		}

	}
}
