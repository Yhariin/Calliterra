#pragma once

class Step;
class Drawable;

class Technique
{
public:
	Technique() = default;
	Technique(std::string name);

	void AddStep(Step step);
	void Submit() const;
	void InitializeParentReferences(const Drawable& parent);
	void Link();

private:
	std::vector<Step> m_Steps;
	std::string m_Name;
};

