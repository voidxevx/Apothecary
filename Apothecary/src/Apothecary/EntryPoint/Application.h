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
		void TONIC_LINK Update(double deltaTime);

		static inline Application& Get() { return *s_Instance; }
		inline lithium::Window& GetWindow() { return *m_Window; }

	private:

		std::unique_ptr<lithium::Window> m_Window;
		bool m_Running = true;

		static Application* s_Instance;

		double m_LastTime = 0;
		double m_DeltaTime = 0;
	};

}