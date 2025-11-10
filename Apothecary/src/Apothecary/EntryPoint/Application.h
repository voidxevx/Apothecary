#pragma once
#include "../Core.h"

#include "../Lithium/Window.h"

#include <memory>

namespace apothec
{

	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		// Defined in Tonic
		void TONIC_LINK Init();
		void TONIC_LINK Destroy();

		static inline Application& Get() { return *s_Instance; }
		inline lithium::Window& GetWindow() { return *m_Window; }

	private:

		std::unique_ptr<lithium::Window> m_Window;
		bool m_Running = true;

		static Application* s_Instance;
	};

}