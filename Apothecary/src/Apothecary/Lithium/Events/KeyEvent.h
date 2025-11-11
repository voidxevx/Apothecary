#pragma once

#include "Event.h"

#include <sstream>

namespace apothec::lithium::events
{

	class KeyEvent : public Event
	{
	public:
		inline const int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategory_Input | EventCategory_Keyboard)
	protected:
		KeyEvent(int keyCode)
			: m_KeyCode(keyCode)
		{}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, unsigned int repeateCount)
			: KeyEvent(keyCode)
			, m_RepeatCount(repeateCount)
		{}

		inline unsigned int GetRepeateCount() const { return m_RepeatCount; }

		virtual std::string ToString() const override
		{
			std::stringstream ss{};
			ss << "Key Pressed Event: (" << m_KeyCode << ") (" << m_RepeatCount << ")" << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		unsigned int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode)
		{}

		virtual std::string ToString() const override
		{
			std::stringstream ss{};
			ss << " Key Released Event: (" << m_KeyCode << ")" << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

}