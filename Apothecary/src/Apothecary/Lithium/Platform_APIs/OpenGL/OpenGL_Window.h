#pragma once

#include "../../Window.h"
#include "Apothecary/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace apothec::lithium::opengl
{

	class Window_OpenGL : public Window
	{
	public:
		Window_OpenGL(const winProps& props);
		virtual ~Window_OpenGL();

		virtual void PreRender() override final;
		virtual void PostRender() override final;

		virtual inline size_t GetWidth() const override final { return m_Data.props.Width; }
		virtual inline size_t GetHeight() const override final { return m_Data.props.Height; }

		virtual void* const GetRawWindow() const override final { return m_Window; }

		virtual inline double GetTime() const override final { return glfwGetTime(); }

		virtual inline void SetEventCallback(const EventCallback &callback) override final { m_Data.callback = callback; }

	private:

		GLFWwindow* m_Window;

		struct WindowData
		{
			winProps props;
			EventCallback callback;
		}
		m_Data;

	};

}