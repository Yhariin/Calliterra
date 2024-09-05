#include "pch.h"
#include "Topology.h"
#include "Renderer.h"

const std::string Topology::GenerateUID(PrimitiveTopology primitiveTopology)
{
    using namespace std::string_literals;
    return typeid(Topology).name() + "#"s + std::to_string(static_cast<int>(primitiveTopology));
}

std::shared_ptr<Topology> Topology::Resolve(PrimitiveTopology primtiveTopology)
{
	return Renderer::GetResourceLibrary().Resolve<Topology>(primtiveTopology);
}
