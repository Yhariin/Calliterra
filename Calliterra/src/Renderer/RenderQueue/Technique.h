#pragma once

class Step;
class Drawable;

class Technique
{
public:
	Technique() = default;

	void AddStep(Step step);
	void Submit() const;
	void InitializeParentReferences(const Drawable& parent);

private:
	std::vector<Step> m_Steps;
};

