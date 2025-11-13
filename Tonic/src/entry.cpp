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
		std::cout << "Layer created\n";
	}

	virtual void OnDetach() override
	{
		std::cout << "Layer destroyed\n";
	}

};

void TONIC_API
apothec::Application::Init()
{
	std::cout << "Hello, Tonic" << std::endl;
	TestLayer* layer = new TestLayer();
	PushLayer(layer);
}

void TONIC_API
apothec::Application::Destroy()
{
	std::cout << "Goodbye, Tonic" << std::endl;
}

void TONIC_API
apothec::Application::Update(double deltaTime)
{

}