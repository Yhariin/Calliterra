#pragma once

class Sink;
class Source;

class Pass
{
public:
	Pass(std::string name);
	virtual ~Pass() = default;

	virtual void Execute() const = 0;
	virtual void Reset();

	const std::string& GetName() const;
	const std::vector<std::unique_ptr<Sink>>& GetSinks() const;

	Source& GetSource(const std::string& name) const;
	Sink& GetSink(const std::string& name) const;

	void SetSinkLinkage(const std::string& sinkName, const std::string& target);
	virtual void Finalize();

protected:
	void RegisterSink(std::unique_ptr<Sink> sink);
	void RegisterSource(std::unique_ptr<Source> source);

private:
	std::vector<std::unique_ptr<Sink>> m_Sinks;
	std::vector<std::unique_ptr<Source>> m_Sources;
	std::string m_Name;
};

