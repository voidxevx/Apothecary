#include "Application.h"

namespace apothec
{

	Application::Application()
	{
		// application construction


		// tonic side construction
		this->Init();
	}

	Application::~Application()
	{
		// application deconstruction


		// tonic side deconstruction
		this->Destroy();
	}

}