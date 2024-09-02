#include "pch.h"
#include "Pass.h"
#include "Renderer/RenderGraph/Sink.h"
#include "Renderer/RenderGraph/Source.h"
#include "Core/Utils.h"

Pass::Pass(std::string name)
	: m_Name(std::move(name))
{
}

void Pass::Reset()
{
}

const std::string& Pass::GetName() const
{
	return m_Name;
}

const std::vector<std::unique_ptr<Sink>>& Pass::GetSinks() const
{
	return m_Sinks;
}

Source& Pass::GetSource(const std::string& name) const
{
	for (auto& source : m_Sources)
	{
		if (source->GetName() == name)
		{
			return *source;
		}
	}
	
	ASSERT(false, "Source not found in pass");
}

Sink& Pass::GetSink(const std::string& name) const
{
	for (auto& sink : m_Sinks)
	{
		if (sink->GetName() == name)
		{
			return *sink;
		}
	}
	
	ASSERT(false, "Sink not found in pass");
}

void Pass::SetSinkLinkage(const std::string& sinkName, const std::string& target)
{
	Sink& sink = GetSink(sinkName);
	auto targetSplit = SplitString(target, ".");

	ASSERT(targetSplit.size() == 2, "Input target has incorrect format");

	sink.SetTarget(std::move(targetSplit[0]), std::move(targetSplit[1]));
}

void Pass::Finalize()
{
	for (auto& sink : m_Sinks)
	{
		sink->PostLinkValidate();
	}
	for (auto& source : m_Sources)
	{
		source->PostLinkValidate();
	}
}

void Pass::RegisterSink(std::unique_ptr<Sink> sink)
{
	// Check for overlap of sink names
	auto validate = [&sink, this]()
	{
		for (auto& s : m_Sinks)
		{
			ASSERT(s->GetName() != sink->GetName(), "Registered sink overlaps with existing");
		}
	};
	STRIP_DEBUG(validate());

	m_Sinks.push_back(std::move(sink));
}

void Pass::RegisterSource(std::unique_ptr<Source> source)
{
	// Check for overlap of source names
	auto validate = [&source, this]()
	{
		for (auto& s : m_Sources)
		{
			ASSERT(s->GetName() != source->GetName(), "Registered source overlaps with existing");
		}
	};
	STRIP_DEBUG(validate());

	m_Sources.push_back(std::move(source));
}
