#include "pch.h"
#include "Application.h"

int main(int argc, char** argv)
{
	Log::Init();

	auto app = new Application();

	app->Run();

	delete app;
}
