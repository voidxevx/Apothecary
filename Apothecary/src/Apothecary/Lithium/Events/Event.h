#pragma once

#include <functional>

namespace apothec::lithium::events
{
	#define BIT(x) 1 << x
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize,                                              // Window Events
		AppTick, AppUpdate, AppRender,                                          // application events
		KeyPressed, KeyReleased,                                                // key events
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled      // mouse events
	};

	enum EventCategory
	{
		None = 0,
		EventCategory_Application           = BIT(0),
		EventCategory_Input                 = BIT(1),
		EventCategory_Keyboard              = BIT(2),
		EventCategory_Mouse                 = BIT(3),
		EventCategory_MouseButton           = BIT(4),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
							   virtual EventType GetEventType() const override { return GetStaticType(); }\
	                           virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
		friend class EventDispatcher;

	public:
		virtual const char* GetName() const = 0;
		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;

		inline bool IsInCategory(EventCategory category) const
		{
			return GetCategoryFlags() & category;
		}
		bool Handled = false;
	};

	class EventDispatcher
	{
		template<typename _T>
		using EventFn = std::function<bool(_T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename _T>
		bool Dispatch(EventFn<_T> func)
		{
			if (m_Event.GetEventType() == _T::GetStaticType())
			{
				m_Event.Handled = func(*(_T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};


}