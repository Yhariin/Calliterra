#pragma once

class Bindable;
class BufferResource;

class Source
{
public:
	virtual ~Source() = default;

	const std::string& GetName() const;
	virtual void PostLinkValidate() const = 0;
	virtual std::shared_ptr<Bindable> YieldBindable();
	virtual std::shared_ptr<BufferResource> YieldBuffer();

protected:
	Source(std::string name);

private:
	std::string m_Name;
};

template<class Type>
class DirectBufferSource : public Source
{
public:
	static std::unique_ptr<DirectBufferSource> Make(std::string name, std::shared_ptr<Type>& buffer)
	{
		return std::make_unique<DirectBufferSource>(std::move(name), buffer);
	}

	DirectBufferSource(std::string name, std::shared_ptr<Type>& buffer)
		: Source(std::move(name)), m_Buffer(buffer)
	{
	}

	void PostLinkValidate() const override
	{
	}

	std::shared_ptr<BufferResource> YieldBuffer() override
	{
		ASSERT(!m_Linked, "Source buffer bound twice");
		m_Linked = true;

		return m_Buffer;
	}

private:
	std::shared_ptr<Type>& m_Buffer;
	bool m_Linked = false;
};

template<class Type>
class DirectBindableSource : public Source
{
public:
	static std::unique_ptr<DirectBindableSource> Make(std::string name, std::shared_ptr<Type>& buffer)
	{
		return std::make_unique<DirectBindableSource>(std::move(name), buffer);
	}

	DirectBindableSource(std::string name, std::shared_ptr<Type>& bind)
		: Source(std::move(name)), m_Bind(bind)
	{
	}

	void PostLinkValidate() const
	{
	}

	std::shared_ptr<Bindable> YieldBindable() override
	{
		return m_Bind;
	}

private:
	std::shared_ptr<Type>& m_Bind;
};


