#pragma once
#include "Event.h"

#include <sstream>

namespace apothec::lithium::events
{

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		~WindowCloseEvent() {}

		virtual std::string ToString() const override
		{
			std::stringstream ss{};
			ss << "Window Closed" << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategory_Application)
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width)
			, m_Height(height)
		{ }
		~WindowResizeEvent()
		{ }

		virtual std::string ToString()const override
		{
			std::stringstream ss{};
			ss << "Window Resized - width: " << m_Width << " height: " << m_Height << std::endl;
			return ss.str();
		}

		const unsigned int GetWidth() const { return m_Width; }
		const unsigned int GetHeight() const { return m_Height; }

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategory_Application)
	private:
		unsigned int m_Width, m_Height;
	};

}