#pragma once

#include "EventLayer.h"

#include <stdint.h>

namespace apothec
{

	struct LayerStack
	{
		EventLayer* ThisLayer;
		LayerStack* NextLayer;
		LayerStack* LastLayer;
		// The layer will automatically detach itself from the stack apon deletion.
		bool AutoDetach = true;

		LayerStack(EventLayer* layer)
			: ThisLayer(layer)
			, NextLayer(nullptr)
			, LastLayer(nullptr)
		{}

		~LayerStack();

		/*
		 * Deletes all layers apart of the stack
		 */
		void PurgeStack(LayerStack*& storage);

		void PurgeStack_Up();
		void PurgeStack_Down();

		/*
		 * Propogates updates in both directions from the called layer
		 */
		void PropogateUpdate(double deltaTime);

		/*
		 *  Propogates event calls downward (next layer.) Stops apon the event being handled
		 */
		void PropogateEvent(lithium::events::Event& event);

		/*
		 * Calls Update layer on all layers in a direction
		 * Up: LastLayer
		 * Down: NextLayer
		 */
		void Pass_OnUpdate_Up(double deltaTime); void Pass_OnUpdate_Down(double deltaTime);

		/*
		 * Pushes a layer to the end of the stack (last to render.) 
		 */
		void PushLayer(EventLayer* layer);

		/*
		 * Detaches layer from the stack 
		 */
		void Detach();
	};

}