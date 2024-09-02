#pragma once
#include "Base/Pass.h"

class BufferResource;

class BufferClearPass : public Pass
{
public:
	BufferClearPass(std::string name);
	void Execute() const override;

private:
	std::shared_ptr<BufferResource> m_Buffer;
};
