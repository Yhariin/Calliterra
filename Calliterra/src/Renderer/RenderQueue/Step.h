#pragma once
#include "Renderer/Bindable.h"
#include "Renderer/IndexBuffer.h"

class Drawable;
class RenderQueuePass;

class Step
{
public:
	Step(std::string m_TargetPassName);
	
	void AddBindable(std::shared_ptr<Bindable> bindable);
	void AddBindable(std::shared_ptr<IndexBuffer> bindable);
	void AddBindables(const std::vector<std::shared_ptr<Bindable>>& bindables);
	void SetIndexCount(uint32_t indexCount);
	void Submit() const ;
	void Execute() const;
	void InitializeParentReferences(const Drawable& parent);
	void Link();

private:
	RenderQueuePass* m_TargetPass = nullptr;
	std::string m_TargetPassName;
	std::vector<std::shared_ptr<Bindable>> m_Bindables;
	uint32_t m_IndexCount = 0;
};

