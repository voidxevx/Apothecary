#include "LayerStack.h"

namespace apothec
{

	LayerStack::LayerStack()
	{}

	LayerStack::~LayerStack()
	{
		if (m_TopLayer)
			m_TopLayer->PurgeLayers();
	}

	void
	LayerStack::PushLayer(EventLayer* layer)
	{
		LayerContainer* cont = new LayerContainer(layer);
		if (!m_TopLayer && !m_InsertLayer) // no layers created
		{
			m_TopLayer = cont;
			m_InsertLayer = m_TopLayer;
		}
		else if (!m_InsertLayer) // the only layer is an overlay
		{
			m_TopLayer->lastLayer = cont;
			m_InsertLayer = cont;
			cont->nextLayer = m_TopLayer;
		}
		else // insert layer
		{
			cont->lastLayer = m_InsertLayer;
			cont->nextLayer = m_InsertLayer->nextLayer;
			if (cont->nextLayer)
				cont->nextLayer->lastLayer = cont;
			m_InsertLayer->nextLayer = cont;
			m_InsertLayer = cont;
			++m_TotalLayers;
		}
	}

	void
	LayerStack::PushOverlay(EventLayer* overlay)
	{
		LayerContainer* cont = new LayerContainer(overlay);
		if (!m_TopLayer && !m_InsertLayer) // no layers
		{
			m_TopLayer = cont;
		}
		else if (m_TopLayer) // push overlay to the top - if is only for dereferenced top layer warning
		{
			cont->lastLayer = m_TopLayer;
			m_TopLayer->nextLayer = cont;
			m_TopLayer = cont;
			++m_TotalLayers;
		}
	}

	void 
	LayerStack::RemoveLayer(EventLayer* layer)
	{
		if (layer == m_TopLayer->thisLayer) // only one layer
		{
			delete layer;
			m_TopLayer = nullptr;
		}
		else if(m_TopLayer->TryRemoveLayer(layer))
			--m_TotalLayers;
	}

	void 
	LayerStack::PropogateUpdate()
	{
		if (m_TopLayer)
			m_TopLayer->UpdateLayer();
	}

	void
	LayerStack::PropogateEvent(lithium::events::Event& event)
	{
		if (m_TopLayer)
			m_TopLayer->OnEvent(event);
	}


	void
	LayerContainer::PurgeLayers()
	{
		if (lastLayer)
			lastLayer->PurgeLayers();

		delete this;
	}

	bool
	LayerContainer::TryRemoveLayer(EventLayer* layer)
	{
		if (layer == thisLayer)
		{
			thisLayer->OnDetach();
			delete thisLayer;
			if (nextLayer)
				nextLayer->lastLayer = lastLayer;
			if (lastLayer)
				lastLayer->nextLayer = nextLayer;
			return true;
		}
		else if (nextLayer)
			return nextLayer->TryRemoveLayer(layer);
		else
			return false;
	}

	void
	LayerContainer::UpdateLayer()
	{
		thisLayer->OnUpdate();
		if (nextLayer)
			nextLayer->UpdateLayer();
	}

	void 
	LayerContainer::OnEvent(lithium::events::Event& event)
	{
		thisLayer->OnEvent(event);
		bool handled = event.Handled;
		if (!handled && nextLayer)
			nextLayer->OnEvent(event);
	}

}