#include "pch.h"
#include "Application.h"

int main(int argc, char** argv)
{
	STRIP_DEBUG(Log::Init());

	Application app;

	app.Run();
}
