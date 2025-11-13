#pragma once

#include "../Layers/EventLayer.h"

namespace apothec::debug
{

	class DebugLayer : public EventLayer
	{
	public:
		DebugLayer();
		~DebugLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(double deltaTime) override;
		virtual void OnEvent(lithium::events::Event& event) override;

	private:
	};

}