#pragma once

#include "Event.h"

namespace apothec::lithium::events
{

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent()
		{}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategory_Application)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() 
		{}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategory_Application)
	};

}