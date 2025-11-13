#pragma once

#include "EventLayer.h"

#include <stdint.h>

namespace apothec
{

	struct LayerContainer
	{
		EventLayer* const thisLayer;
		LayerContainer* nextLayer = nullptr; // down
		LayerContainer* lastLayer = nullptr; // up

		LayerContainer(EventLayer* layer)
			: thisLayer(layer)
		{}

		void PurgeLayers();
		void UpdateLayer(double deltaTime);
		void OnEvent(lithium::events::Event& event);

		bool TryRemoveLayer(EventLayer* layer);
	};

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PropogateUpdate(double deltaTime);
		void PropogateEvent(lithium::events::Event& event);

		void PushLayer(EventLayer* layer);
		void PushOverlay(EventLayer* overlay);
		void RemoveLayer(EventLayer* layer);

	private:
		LayerContainer* m_TopLayer = nullptr; // first layer 
		LayerContainer* m_InsertLayer = nullptr; // layer to insert to
		size_t m_TotalLayers = 0;

	};

}