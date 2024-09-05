#pragma once
#include "Renderer/Bindable.h"
#include "Renderer/IndexBuffer.h"

class Drawable;
enum class PassName;

class Step
{
public:
	Step(PassName m_TargetPass);
	
	void AddBindable(std::shared_ptr<Bindable> bindable);
	void AddBindable(std::shared_ptr<IndexBuffer> bindable);
	void AddBindables(const std::vector<std::shared_ptr<Bindable>>& bindables);
	void SetIndexCount(uint32_t indexCount);
	void Submit() const ;
	void Execute() const;
	void InitializeParentReferences(const Drawable& parent);

private:
	PassName m_TargetPass;
	std::vector<std::shared_ptr<Bindable>> m_Bindables;
	uint32_t m_IndexCount = 0;
};

