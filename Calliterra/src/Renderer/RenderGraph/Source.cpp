#include "pch.h"
#include "Source.h"

Source::Source(std::string name)
	: m_Name(std::move(name))
{
	ASSERT(!m_Name.empty(), "Empty source name");

	auto validate = [this]()
		{
			const bool nameCharsValid = std::all_of(m_Name.begin(), m_Name.end(), [](char c)
				{
					return std::isalnum(c) || c == '_';
				});

			ASSERT(nameCharsValid && !std::isdigit(m_Name.front()), "Invalid source name");
		};

	STRIP_DEBUG(validate());
}

const std::string& Source::GetName() const
{
	return m_Name;
}

std::shared_ptr<Bindable> Source::YieldBindable()
{
	ASSERT("Source cannot be accessed as bindable");
	return nullptr;
}

std::shared_ptr<BufferResource> Source::YieldBuffer()
{
	ASSERT("Source cannot be accessed as buffer");
	return nullptr;
}

