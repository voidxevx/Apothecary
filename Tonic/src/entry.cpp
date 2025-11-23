#include <Apothec.h>
#include "Core.h"

#include <iostream>

void TONIC_API
apothec::Application::Init()
{
	new bismuth::state{};
	new bismuth::DataRegistry{};
	new bismuth::IntDataType{};
	new bismuth::StringDataType{};
	new bismuth::EntityPtrDataType{};

	bismuth::state::GetGlobal()->BuildFile("../Content/Tonic/temp.bis");
}

void TONIC_API
apothec::Application::DebugInit()
{

}

void TONIC_API
apothec::Application::Destroy()
{

}

void TONIC_API
apothec::Application::Update(double deltaTime)
{

}