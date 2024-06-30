#pragma once

class RendererAPI
{
public:
	enum API
	{
		None = 0, DX11 = 1
	};

	virtual ~RendererAPI() = default;
	virtual void Init() = 0;

	static API GetAPI() { return s_API; }
	static std::unique_ptr<RendererAPI> Create();
private:
	inline static API s_API = DX11;
};

