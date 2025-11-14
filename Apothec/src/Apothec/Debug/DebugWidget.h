#pragma once

#include <string>

namespace apothec::debug
{

	class DebugWidget
	{
	public:

		/* Detaches itself from the chain */
		virtual 
		~DebugWidget()
		{
			if (m_ShouldDetach)
				Detach();
		}

		

		/*
		 * Widgets automatically call imgui's begin and end functions.
		 * Calls update for the next widget in the chain.
		 */
		void WidgetUpdate(double deltaTime);

		// Declared in single implementations
		virtual void OnUpdate(double deltaTime) = 0;

		/*
		 * Detaches itself from a widget chain 
		 */
		void 
		Detach()
		{
			if (m_NextWidget)
				m_NextWidget->SetLastWidget(m_LastWidget);
			if (m_LastWidget)
				m_LastWidget->SetNextWidget(m_NextWidget);

			m_NextWidget = nullptr;
			m_LastWidget = nullptr;
		}

		/*
		 * Adds a widget to the front of the widget chain
		 */
		inline void
		AppendWidget(DebugWidget* next)
		{
			if (m_NextWidget)
				m_NextWidget->AppendWidget(next);
			else
			{
				m_NextWidget = next;
				next->SetLastWidget(this);
			}
		}

		inline void SetNextWidget(DebugWidget* next) { m_NextWidget = next; }
		inline void SetLastWidget(DebugWidget* last) { m_LastWidget = last; }

		/*
		 * Starts a recursion of widget deletion both down and up the chain. 
		 */
		void 
		PurgeChain()
		{
			m_NextWidget->PurgeChain_Down();
			m_LastWidget->PurgeChain_Up();

			delete this;
		}

		/*
		 * Deletes itself and tells the chain before it to do the same.
		 */
		void 
		PurgeChain_Up()
		{
			m_ShouldDetach = false;
			if (m_LastWidget)
				m_LastWidget->PurgeChain_Up();

			m_LastWidget = nullptr;
			delete this;
		}

		/*
		 * Deletes itself and tells the next widget to do the same. 
		 */
		void 
		PurgeChain_Down()
		{
			m_ShouldDetach = false;
			if (m_NextWidget)
				m_NextWidget->PurgeChain_Down();

			m_NextWidget = nullptr;
			delete this;
		}

		/*
		 * Sets the rendering of the widget to be seperate from other widgets in its chain
		 */
		void 
		SetSingleWidgetRendering(bool val = false)
		{
			m_SingleWidget = val;
		}

	protected:
		DebugWidget(const std::string& name)
			: m_WidgetName(name)
		{}

		std::string m_WidgetName;

	private:
		DebugWidget* m_NextWidget = nullptr;
		DebugWidget* m_LastWidget = nullptr;
		bool m_ShouldDetach = true;
		bool m_SingleWidget = true;
	};

}