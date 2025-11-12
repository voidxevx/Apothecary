#pragma once
#include "Apothecary/Core.h"

#include "Apothecary/Lithium/Window.h"
#include "Apothecary/Lithium/Events/LithiumEvents.h"
#include "Layers/LayerStack.h"

#include <Apothecary/Tools/Xenon/src/Xenon.h>

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

		inline void PushLayer(EventLayer* layer)
		{
			m_LayerStack.PushLayer(layer);
		}
		inline void PushOverlay(EventLayer* layer)
		{
			m_LayerStack.PushOverlay(layer);
		}
		inline void RemoveLayer(EventLayer* layer)
		{
			m_LayerStack.RemoveLayer(layer);
		}

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
		LayerStack m_LayerStack;
		bool m_Running = true;

		static Application* s_Instance;

		double m_LastTime = 0;
		double m_DeltaTime = 0;

		xenon::registry* m_EntityRegistry;
	};

}