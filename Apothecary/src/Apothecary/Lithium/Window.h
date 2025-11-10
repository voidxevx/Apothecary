#pragma once

#include <string>

namespace apothec::lithium
{

	struct winProps
	{
		std::string Name;
		unsigned int Width, Height;

		winProps(std::string name = "Apothecary", unsigned int width = 1280, unsigned int height = 720)
			: Name(name)
			, Width(width)
			, Height(height)
		{}

	};

	class Window
	{
	public:
		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual inline size_t GetWidth() const = 0;
		virtual inline size_t GetHeight() const = 0;

		// virtual void SetEventCallback(const EventCallback &callback) = 0;

		static Window* CreateWindow(const winProps& props = winProps());
	};

}