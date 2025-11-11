#pragma once

#include "Event.h"

#include <sstream>

namespace apothec::lithium::events
{

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent()
		{}

		virtual std::string ToString() const override
		{
			std::stringstream ss{};
			ss << "App tick event" << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategory_Application)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() 
		{}

		virtual std::string ToString() const override
		{
			std::stringstream ss{};
			ss << "App update Event" << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategory_Application)
	};

}