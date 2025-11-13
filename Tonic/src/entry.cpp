#include <Apothec.h>
#include "Core.h"

#include <iostream>

class TONIC_API TestLayer : public apothec::EventLayer
{
public:
	TestLayer()
	{

	}

	virtual ~TestLayer()
	{

	}

	virtual void OnAttach() override
	{
	}

	virtual void OnDetach() override
	{
	}

};

void TONIC_API
apothec::Application::Init()
{
	TestLayer* layer = new TestLayer();
	PushLayer(layer);
}

void TONIC_API
apothec::Application::Destroy()
{

}

void TONIC_API
apothec::Application::Update(double deltaTime)
{

}