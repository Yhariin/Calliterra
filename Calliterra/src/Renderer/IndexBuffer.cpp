#include "pch.h"
#include "IndexBuffer.h"
#include "Renderer.h"

std::shared_ptr<IndexBuffer> IndexBuffer::Resolve(const std::string& tag, const std::vector<uint32_t>& indices)
{
	ASSERT(tag != "?");
	return Renderer::GetResourceLibrary().Resolve<IndexBuffer>(tag, indices);
}
