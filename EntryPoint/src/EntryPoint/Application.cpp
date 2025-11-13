#include "Application.h"

#include "Debug/DebugLayer.h"

namespace apothec
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		// application construction
		s_Instance = this;
		m_Window = std::unique_ptr<lithium::Window>(lithium::Window::CreateWindow());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		debug::DebugLayer* debug = new debug::DebugLayer();
		PushLayer(debug);

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

			// tonic side update
			this->Update(m_DeltaTime);

			m_Window->PreRender();

			// render calls
			m_LayerStack.PropogateUpdate(m_DeltaTime);

			// update ecs systems

			m_Window->PostRender();
		}
	}

	void 
	Application::CloseApplication()
	{
		lithium::events::WindowCloseEvent event{};
		this->OnWindowClose(event);
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
		m_LayerStack.PropogateEvent(event);
	}

}