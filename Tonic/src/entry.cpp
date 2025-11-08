#include <Apothec.h>
#include "Core.h"

#include <iostream>

void TONIC_API
apothec::Application::Init()
{
	std::cout << "Hello, Tonic" << std::endl;
}

void TONIC_API
apothec::Application::Destroy()
{
	std::cout << "Goodbye, Tonic" << std::endl;
}