#include "pch.h"
#include "Sink.h"

Sink::Sink(std::string name)
	: m_Name(std::move(name))
{
	ASSERT(!m_Name.empty(), "Empty sink name");

	auto validate = [this]()
		{
			const bool nameCharsValid = std::all_of(m_Name.begin(), m_Name.end(), [](char c)
				{
					return std::isalnum(c) || c == '_';
				});

			ASSERT(nameCharsValid && !std::isdigit(m_Name.front()), "Invalid sink name");
		};

	STRIP_DEBUG(validate());
}

const std::string& Sink::GetName() const
{
	return m_Name;
}

const std::string& Sink::GetPassName() const
{
	return m_PassName;
}

const std::string& Sink::GetSourceName() const
{
	return m_SourceName;
}

void Sink::SetTarget(std::string passName, std::string sourceName)
{
	ASSERT(!passName.empty(), "Empty output name");

	auto validatePassName = [&passName]()
		{
			const bool nameCharsValid = std::all_of(passName.begin(), passName.end(), [](char c)
				{
					return std::isalnum(c) || c == '_';
				});

			ASSERT(passName == "$" || (nameCharsValid && !std::isdigit(passName.front())), "Invalid sink name");
		};

	auto validateOutputName = [&sourceName]()
		{
			const bool nameCharsValid = std::all_of(sourceName.begin(), sourceName.end(), [](char c)
				{
					return std::isalnum(c) || c == '_';
				});

			ASSERT(nameCharsValid && !std::isdigit(sourceName.front()), "Invalid sink name");
		};

	STRIP_DEBUG(validatePassName());
	this->m_PassName = passName;
	STRIP_DEBUG(validateOutputName());
	this->m_SourceName = sourceName;
}

