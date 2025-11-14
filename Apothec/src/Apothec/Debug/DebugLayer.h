#pragma once

#include "../Layers/EventLayer.h"
#include "DebugWidget.h"

namespace apothec::debug
{


	class DebugLayer : public EventLayer
	{
	public:
		DebugLayer();
		~DebugLayer();

		virtual void OnAttach() override final;
		virtual void OnDetach() override final;
		virtual void OnUpdate(double deltaTime) override final;
		virtual void OnEvent(lithium::events::Event& event) override final;

		void AddWidget(DebugWidget* widget);

		static void CreateDebugLayer();
		static inline DebugLayer* const Get() { return s_Instance; }

	private:
		DebugWidget* m_WidgetChain;
		static DebugLayer* s_Instance;

	};


}