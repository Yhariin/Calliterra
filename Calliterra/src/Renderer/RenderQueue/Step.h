#pragma once
#include "Renderer/Bindable.h"
#include "Renderer/IndexBuffer.h"

class Drawable;

class Step
{
public:
	Step(uint32_t targetPass);
	
	void AddBindable(std::shared_ptr<Bindable> bindable);
	void AddBindable(std::shared_ptr<IndexBuffer> bindable);
	void AddBindables(const std::vector<std::shared_ptr<Bindable>>& bindables);
	void SetIndexCount(uint32_t indexCount);
	void Submit() const ;
	void Execute() const;
	void InitializeParentReferences(const Drawable& parent);

private:
	uint32_t m_TargetPass;
	uint32_t m_IndexCount = 0;
	std::vector<std::shared_ptr<Bindable>> m_Bindables;
};

