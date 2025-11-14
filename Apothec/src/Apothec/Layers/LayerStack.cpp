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
		layer->OnAttach();
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
		overlay->OnAttach();
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
		if (layer)
			layer->OnDetach();
		if (layer == m_TopLayer->thisLayer) // only one layer
		{
			delete layer;
			m_TopLayer = nullptr;
			m_InsertLayer = nullptr;
		}
		else if(m_TopLayer->TryRemoveLayer(layer))
			--m_TotalLayers;
	}

	void 
	LayerStack::PropogateUpdate(double deltaTime)
	{
		if (m_TopLayer)
			m_TopLayer->UpdateLayer(deltaTime);
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
	LayerContainer::UpdateLayer(double deltaTime)
	{
		thisLayer->OnUpdate(deltaTime);
		if (nextLayer)
			nextLayer->UpdateLayer(deltaTime);
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