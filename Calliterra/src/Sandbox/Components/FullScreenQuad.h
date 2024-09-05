#pragma once
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"

// TODO: Implement as a bindable
class FullScreenQuad
{
public:
	FullScreenQuad();
	
	void Bind() const;
	
	void Draw() const;
	
private:
	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	std::shared_ptr<Shader> m_VertexShader;
	std::shared_ptr<Shader> m_PixelShader;
};

