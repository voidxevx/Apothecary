#include "Application.h"

namespace apothec
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		// application construction
		s_Instance = this;
		m_Window = std::unique_ptr<lithium::Window>(lithium::Window::CreateWindow());
		// bind events

		// tonic side construction
		this->Init();
	}

	Application::~Application()
	{
		// application deconstruction


		// tonic side deconstruction
		this->Destroy();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			// iterate over layers
			// update ecs systems
			m_Window->OnUpdate();
		}
	}

}