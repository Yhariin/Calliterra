#pragma once
#include "Renderer/Bindable.h"
class Drawable;

class Step
{
public:
	Step(uint32_t targetPass);
	
	void AddBindable(std::shared_ptr<Bindable> bindable);
	void SetIndexCount(uint32_t indexCount);
	void Submit() const ;
	void Execute() const;
	void InitializeParentReferences(const Drawable& parent);

private:
	uint32_t m_TargetPass;
	uint32_t m_IndexCount;
	std::vector<std::shared_ptr<Bindable>> m_Bindables;
};

