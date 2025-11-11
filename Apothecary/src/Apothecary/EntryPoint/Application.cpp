#include "Application.h"

namespace apothec
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		// application construction
		s_Instance = this;
		m_Window = std::unique_ptr<lithium::Window>(lithium::Window::CreateWindow());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		// tonic side construction
		this->Init();
	}

	Application::~Application()
	{
		// application deconstruction


		// tonic side deconstruction
		this->Destroy();
	}

	void 
	Application::Run()
	{
		while (m_Running)
		{
			double time = m_Window->GetTime();
			m_DeltaTime = time - m_LastTime;
			m_LastTime = time;

			this->Update(m_DeltaTime);

			// iterate over layers
			// update ecs systems
			m_Window->OnUpdate();
		}
	}

	bool
	Application::OnWindowClose(lithium::events::WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	void
	Application::OnEvent(lithium::events::Event& event)
	{
		lithium::events::EventDispatcher disp(event);
		disp.Dispatch<lithium::events::WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

		// layer events

	}

}