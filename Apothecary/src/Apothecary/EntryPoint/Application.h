#pragma once
#include "../Core.h"

namespace apothec
{

	class Application
	{
	public:
		Application();
		~Application();

		// Defined in Tonic
		void TONIC_LINK Init();
		void TONIC_LINK Destroy();
	};

}