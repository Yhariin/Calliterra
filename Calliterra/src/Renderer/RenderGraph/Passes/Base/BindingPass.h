#pragma once
#include "Pass.h"
#include "Renderer/RenderGraph/Sink.h"

class Bindable;
class RenderTarget;
class DepthStencilBuffer;

class BindingPass : public Pass
{
protected:
	BindingPass(std::string name, std::vector<std::shared_ptr<Bindable>> binds = {});
	void AddBind(std::shared_ptr<Bindable> bind);
	void BindAll() const;
	void Finalize();

protected:
	template<class Type>
	void AddBindSink(std::string name)
	{
		const auto index = m_Binds.size();
		m_Binds.emplace_back();
		RegisterSink(std::make_unique<ContainerBindableSink<Type>> (std::move(name), m_Binds, index));
	}
	std::shared_ptr<RenderTarget> m_RenderTarget;
	std::shared_ptr<DepthStencilBuffer> m_DepthStencilBuffer;

private:
	void BindBufferResources() const;
private:
	std::vector<std::shared_ptr<Bindable>> m_Binds;
};

