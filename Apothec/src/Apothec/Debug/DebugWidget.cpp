#include "DebugWidget.h"

#include "imgui.h"

namespace apothec::debug
{

	void
	DebugWidget::WidgetUpdate(double deltaTime)
	{
		if (m_SingleWidget)
			ImGui::Begin(m_WidgetName.c_str());

		OnUpdate(deltaTime);

		if(m_SingleWidget)
			ImGui::End();
		if (m_NextWidget)
			m_NextWidget->WidgetUpdate(deltaTime);
	}

}