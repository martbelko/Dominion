#pragma once

#include "Dominion/Core/Base.h"

#include <string>

namespace Dominion {

	struct BaseComponent
	{
		std::string name;
		bool isStatic = false;
	};

}
