#pragma once

#include "DebugWidget.h"

#include "../Tools/Argon/src/Argon.h"

namespace apothec::debug::widgets
{

	class DebugSubWidgetChain : public DebugWidget
	{
	public:
		DebugSubWidgetChain(const std::string& label, DebugWidget* subwidget = nullptr)
			: DebugWidget(label)
			, m_SubWidget(subwidget)
		{}

		virtual ~DebugSubWidgetChain()
		{
			m_SubWidget->PurgeChain();
		}

		void
		AddSubWidget(DebugWidget* widget)
		{
			widget->SetSingleWidgetRendering();
			if (m_SubWidget)
				m_SubWidget->AppendWidget(widget);
			else
				m_SubWidget = widget;
		}

		virtual void OnUpdate(double deltaTime) override final;

	private:
		DebugWidget* m_SubWidget;
	};

	class AppManagerWidget : public DebugWidget
	{
	public:
		AppManagerWidget()
			: DebugWidget("Apothecary")
		{}

		virtual void OnUpdate(double deltaTime) override final;
	};

	class Vec2HandleWidget : public DebugWidget
	{
	public:
		Vec2HandleWidget(argon::vec2& vec, const std::string& label = "vec2")
			: DebugWidget(label)
			, m_Vector(vec)
		{}

		virtual void OnUpdate(double deltaTime) override final;

	private:
		argon::vec2& m_Vector;
		float m_SliderRange = 100;
	};

}