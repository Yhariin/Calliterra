#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	virtual ~IndexBuffer() = default;

	template <typename...IgnoreParams>
	static const std::string GenerateUID(const std::string& tag, IgnoreParams&&...ignore)
	{
		using namespace std::string_literals;
		return typeid(IndexBuffer).name() + "#"s + tag;
	}

	static std::shared_ptr<IndexBuffer> Resolve(const std::string& tag, const std::vector<uint32_t>& indices);

	virtual uint32_t GetCount() const = 0;

};

