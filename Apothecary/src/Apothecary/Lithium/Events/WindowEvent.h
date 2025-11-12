#pragma once
#include "Event.h"

namespace apothec::lithium::events
{

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		~WindowCloseEvent() {}

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

		const unsigned int GetWidth() const { return m_Width; }
		const unsigned int GetHeight() const { return m_Height; }

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategory_Application)
	private:
		unsigned int m_Width, m_Height;
	};

}