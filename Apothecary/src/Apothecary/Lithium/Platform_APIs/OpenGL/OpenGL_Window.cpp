#include "OpenGL_Window.h"

#include <iostream>

#include "EntryPoint/Application.h"

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

		m_Data.props.Width = mode->width;
		m_Data.props.Height = mode->height;*/

		m_Window = glfwCreateWindow(m_Data.props.Width, m_Data.props.Height, m_Data.props.Name.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glfwSwapInterval(1);
		glfwSetWindowUserPointer(m_Window, &m_Data);

#define EVENT_GET_DATAPOINTER WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window)

		/* ------ glfw Event Callbacks ------*/
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
		{
			EVENT_GET_DATAPOINTER;

			data.props.Width = width;
			data.props.Height = height;

			lithium::events::WindowResizeEvent event(width, height);
			data.callback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
		{
			EVENT_GET_DATAPOINTER;

			lithium::events::WindowCloseEvent event{};
			data.callback(event);
		});

		//key event -----
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
		{
			EVENT_GET_DATAPOINTER;

			switch (action)
			{
			case GLFW_PRESS:
			{
				lithium::events::KeyPressedEvent eventPressed(key, 0);
				data.callback(eventPressed);
				break;
			}
			case GLFW_RELEASE:
			{
				lithium::events::KeyReleasedEvent eventReleased(key);
				data.callback(eventReleased);
				break;
			}
			case GLFW_REPEAT:
			{
				lithium::events::KeyPressedEvent eventPressed(key, 1);
				data.callback(eventPressed);
				break;
			}
			}
		});
		// mouse event -----
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int modes) 
		{
			EVENT_GET_DATAPOINTER;

			switch (action)
			{
			case GLFW_PRESS:
			{
				lithium::events::MouseButtonPressedEvent eventPressed(button);
				data.callback(eventPressed);
				break;
			}
			case GLFW_RELEASE:
			{
				lithium::events::MouseButtonReleasedEvent eventReleased(button);
				data.callback(eventReleased);
				break;
			}
			}
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) 
		{
			EVENT_GET_DATAPOINTER;

			lithium::events::MouseMoveEvent event(xPos, yPos);
			data.callback(event);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOff, double yOff) 
		{
			EVENT_GET_DATAPOINTER;

			lithium::events::MouseScrolledEvent event(xOff, yOff);
			data.callback(event);
		});

#undef EVENT_GET_DATAPOINTER
		/* ------ -------------------- ------*/


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