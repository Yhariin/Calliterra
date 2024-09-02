#pragma once
#include "Source.h"

//class Pass;

class Sink
{
public:
	virtual ~Sink() = default;

	const std::string& GetName() const;
	const std::string& GetPassName() const;
	const std::string& GetSourceName() const;

	void SetTarget(std::string passName, std::string outputName);
	virtual void Bind(Source& source) = 0;
	virtual void PostLinkValidate() const = 0;

protected:
	Sink(std::string name);

private:
	std::string m_Name;
	std::string m_PassName;
	std::string m_SourceName;
};

template<class Type>
class DirectBufferSink : public Sink
{
	static_assert(std::is_base_of_v<BufferResource, Type>, "DirectBufferSink target type must be a BufferResource type");
public:
	DirectBufferSink(std::string name, std::shared_ptr<Type>& bind)
		: Sink(std::move(name)), m_Target(bind)
	{
	}

	static std::unique_ptr<Sink> Make(std::string name, std::shared_ptr<Type>& target)
	{
		return std::make_unique<DirectBufferSink>(std::move(name), target);
	}

	void PostLinkValidate() const override
	{
		ASSERT(m_Linked, "Unlinked sink");
	}

	void Bind(Source& source) override
	{
		auto buffer = std::dynamic_pointer_cast<Type>(source.YieldBuffer());

		ASSERT(buffer, "Incompatible buffer type");

		m_Target = std::move(buffer);
		m_Linked = true;
	}

private:
	std::shared_ptr<Type>& m_Target;
	bool m_Linked = false;
};

template<class Type>
class ContainerBindableSink : public Sink
{
	static_assert(std::is_base_of_v<BufferResource, Type>, "ContainerBufferSink target type must be a BufferResource type");
public:
	ContainerBindableSink(std::string name, std::vector<std::shared_ptr<Bindable>>& container, uint32_t index)
		: Sink(std::move(name)), m_Container(container), m_Index(index)
	{
	}

	void PostLinkValidate() const override
	{
		ASSERT(m_Linked, "Unlinked sink");
	}

	void Bind(Source& source)
	{
		auto buffer = std::dynamic_pointer_cast<Type>(source.YieldBuffer());

		ASSERT(buffer, "Incompatible buffer type");

		m_Container[m_Index] = std::move(buffer);
		m_Linked = true;
	}

private:
	std::vector<std::shared_ptr<Bindable>>& m_Container;
	uint32_t m_Index;
	bool m_Linked = false;
};

template<class Type>
class DirectBindableSink : public Sink
{
	static_assert(std::is_base_of_v<Bindable, Type>, "DirectBindableSink target type must be a Bindable type");
public:

	DirectBindableSink(std::string name, std::shared_ptr<Type>& target)
		: Sink(std::move(name)), m_Target(target)
	{
	}

	static std::unique_ptr<Sink> Make(std::string name, std::shared_ptr<Type>& target)
	{
		return std::make_unique<DirectBindableSink>(std::move(name), target);
	}

	void PostLinkValidate() const override
	{
		ASSERT(m_Linked, "Unlinked sink");
	}

	void Bind(Source& source) override
	{
		auto bindable = std::dynamic_pointer_cast<Type>(source.YieldBindable());

		ASSERT(bindable, "Incompatible bindable type");

		m_Target = std::move(bindable);
		m_Linked = true;
	}

private:
	std::shared_ptr<Type>& m_Target;
	bool m_Linked = false;
};

