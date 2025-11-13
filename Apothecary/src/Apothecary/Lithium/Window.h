#pragma once

#include "Events/Event.h"

#include <string>
#include <functional>

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
		using EventCallback = std::function<void(events::Event&)>;
		virtual ~Window() {}

		virtual void PreRender() = 0;
		virtual void PostRender() = 0;

		virtual inline size_t GetWidth() const = 0;
		virtual inline size_t GetHeight() const = 0;

		virtual inline double GetTime() const = 0;

		virtual void* const GetRawWindow() const = 0;

		virtual void SetEventCallback(const EventCallback &callback) = 0;

		static Window* CreateWindow(const winProps& props = winProps());
	};

}