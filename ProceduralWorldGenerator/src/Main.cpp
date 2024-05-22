#include <iostream>
#include "Application.hpp"

int main(int argc, char** argv)
{
	auto app = new Application();

	app->Run();

	delete app;
}
