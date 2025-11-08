#include <iostream>

#include "EntryPoint/Application.h"

int
main()
{
	apothec::Application* app = new apothec::Application{};

	delete app;
	return 0;
}