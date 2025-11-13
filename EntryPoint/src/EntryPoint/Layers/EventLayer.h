#pragma once

#include "Apothecary/Lithium/Events/Event.h"

namespace apothec
{

	class EventLayer
	{
	public:
		EventLayer() {}
		virtual ~EventLayer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(double deltaTime) {}
		virtual void OnEvent(lithium::events::Event& event) {}
	};

}