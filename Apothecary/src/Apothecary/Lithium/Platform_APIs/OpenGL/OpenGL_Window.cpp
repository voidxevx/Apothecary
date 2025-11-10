#include "OpenGL_Window.h"

#include <iostream>

#include "Apothecary/EntryPoint/Application.h"

namespace apothec::lithium
{
	Window* Window::CreateWindow(const winProps &props)
	{
		return new opengl::Window_OpenGL(props);
	}
}

namespace apothec::lithium::opengl
{

	static bool s_GLFWInitialized = false;

	static void 
	GLFW_ErrorCallback(int count, const char* desc)
	{
		std::cout << "[" << count << "] " << desc << "\n";
	}

	Window_OpenGL::Window_OpenGL(const winProps &props)
		: m_Window(nullptr)
	{
		m_Data.props = props;

		if (!s_GLFWInitialized)
		{
			int valid = glfwInit();
			if (valid == GLFW_FALSE)
				return;
			glfwSetErrorCallback(GLFW_ErrorCallback);
			s_GLFWInitialized = true;
		}

		/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

		/*GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		m_Data.Width = mode->width;
		m_Data.Height = mode->height;*/

		m_Window = glfwCreateWindow(m_Data.props.Width, m_Data.props.Height, m_Data.props.Name.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glfwSwapInterval(1);
		glfwSetWindowUserPointer(m_Window, &m_Data);

		// TODO: event callbacks




	}

	Window_OpenGL::~Window_OpenGL()
	{
		glfwDestroyWindow(m_Window);
	}

	void 
	Window_OpenGL::OnUpdate()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.35, 0.15, 0.44, 1.0); 

		// render calls

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

}