#pragma once

#include "Event.h"

namespace apothec::lithium::events
{

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(double xPos, double yPos)
			: m_XPosition(xPos)
			, m_YPosition(yPos)
		{}

		inline const double GetXPosition() const { return m_XPosition; }
		inline const double GetYPosition() const { return m_YPosition; }

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategory_Mouse)

	private:
		double m_XPosition, m_YPosition;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(double xOffset, double yOffset)
			: m_XOffset(xOffset)
			, m_YOffset(yOffset)
		{}

		inline const double GetXOffset() const { return m_XOffset; }
		inline const double GetYOffset() const { return m_YOffset; }

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategory_Mouse)
	private:
		double m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline const unsigned int GetButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategory_Input | EventCategory_MouseButton | EventCategory_Mouse)
	protected:
		MouseButtonEvent(unsigned int button)
			: m_Button(button)
		{}

		unsigned int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(unsigned int button)
			: MouseButtonEvent(button)
		{}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(unsigned int button)
			: MouseButtonEvent(button)
		{}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}