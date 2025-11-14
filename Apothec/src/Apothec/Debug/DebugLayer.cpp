#include "DebugLayer.h"

namespace apothec::debug
{

	DebugLayer* DebugLayer::s_Instance = nullptr;

	DebugLayer::DebugLayer()
		: m_WidgetChain(nullptr)
	{
	}

	DebugLayer::~DebugLayer()
	{
	}

	void
	DebugLayer::CreateDebugLayer()
	{
		s_Instance = new DebugLayer();
	}

	void 
	DebugLayer::AddWidget(DebugWidget* widget)
	{
		if (m_WidgetChain)
			m_WidgetChain->AppendWidget(widget);
		else
			m_WidgetChain = widget;
	}

}