#pragma once
#include "Base/RenderPass.h"
//#include "Renderer/BufferResource.h"

class ClearBufferPass : public RenderPass
{
public:
	ClearBufferPass(std::shared_ptr<BufferResource> buffer)
		: m_Buffer(buffer)
	{
	}

	void Execute() const override
	{
		m_Buffer->Clear();
	}

private:
	std::shared_ptr<BufferResource> m_Buffer;
};
