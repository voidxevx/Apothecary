#include "Widgets.h"

#include "../Application.h"

#include "imgui.h"

namespace apothec::debug::widgets
{

	void
	DebugSubWidgetChain::OnUpdate(double deltaTime)
	{
		m_SubWidget->WidgetUpdate(deltaTime);
	}

	void
	AppManagerWidget::OnUpdate(double)
	{
		if (ImGui::Button("Close Apothecary"))
			Application::Get().CloseApplication();
	}

	void
	Vec2HandleWidget::OnUpdate(double)
	{
		ImGui::DragFloat("Range", &m_SliderRange, 15.0f, 0.0f, 1000.0f);
		ImGui::DragFloat2("vec2", m_Vector.items, 1.0f, -m_SliderRange, m_SliderRange);
	}

}