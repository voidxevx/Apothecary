#pragma once
#include "../Core.h"

#include "../Lithium/Window.h"
#include "../Lithium/Events/LithiumEvents.h"

#include <Xenon.h>

#include <memory>

namespace apothec
{

	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		void OnEvent(lithium::events::Event& event);

		// Defined in Tonic
		void TONIC_LINK Init();
		void TONIC_LINK Destroy();
		void TONIC_LINK Update(double deltaTime);

		static inline Application& Get() { return *s_Instance; }
		inline lithium::Window& GetWindow() { return *m_Window; }
		inline xenon::registry& GetRegistry() { return *m_EntityRegistry; }

	private:
		bool OnWindowClose(lithium::events::WindowCloseEvent& event);

		std::unique_ptr<lithium::Window> m_Window;
		bool m_Running = true;

		static Application* s_Instance;

		double m_LastTime = 0;
		double m_DeltaTime = 0;

		xenon::registry* m_EntityRegistry;
	};

}