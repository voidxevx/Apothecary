#include "LayerStack.h"

namespace apothec
{

	LayerStack::~LayerStack()
	{
		if (AutoDetach)
			Detach();

		delete ThisLayer;
	}



	void
	LayerStack::PurgeStack(LayerStack*& storage)
	{
		AutoDetach = false;

		if (NextLayer)
			NextLayer->PurgeStack_Down();
		if (LastLayer)
			LastLayer->PurgeStack_Up();

		storage = nullptr;
		delete this;
	}

	void
	LayerStack::PurgeStack_Up()
	{
		if (LastLayer)
			LastLayer->PurgeStack_Up();

		delete this;
	}

	void
	LayerStack::PurgeStack_Down()
	{
		if (NextLayer)
			NextLayer->PurgeStack_Down();

		delete this;
	}



	void
	LayerStack::PropogateUpdate(double deltaTime)
	{
		ThisLayer->OnUpdate(deltaTime);

		if (LastLayer)
			LastLayer->Pass_OnUpdate_Up(deltaTime);
		if (NextLayer)
			NextLayer->Pass_OnUpdate_Down(deltaTime);
	}

	void
	LayerStack::PropogateEvent(lithium::events::Event& event)
	{
		ThisLayer->OnEvent(event);

		if (NextLayer && event.Handled)
			NextLayer->PropogateEvent(event);
	}


	void
	LayerStack::Pass_OnUpdate_Up(double deltaTime)
	{
		ThisLayer->OnUpdate(deltaTime);

		if (LastLayer)
			LastLayer->Pass_OnUpdate_Up(deltaTime);
	}

	void
	LayerStack::Pass_OnUpdate_Down(double deltaTime)
	{
		ThisLayer->OnUpdate(deltaTime);

		if (NextLayer)
			NextLayer->Pass_OnUpdate_Down(deltaTime);
	}




	void
	LayerStack::PushLayer(EventLayer* layer)
	{
		layer->OnAttach();
		if (NextLayer) // recursive push
			NextLayer->PushLayer(layer);
		else
		{
			LayerStack* newLayer = new LayerStack{ layer };
			NextLayer = newLayer;
			newLayer->LastLayer = this;
		}
	}




	void
	LayerStack::Detach()
	{
		ThisLayer->OnDetach();
		if (LastLayer)
			LastLayer->NextLayer = NextLayer;
		if (NextLayer)
			NextLayer->LastLayer = LastLayer;

		LastLayer = nullptr;
		NextLayer = nullptr;
	}

}