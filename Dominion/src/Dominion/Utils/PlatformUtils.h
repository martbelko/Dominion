#pragma once

#include "Dominion/Core/Base.h"

#include <string>

namespace Dominion {

	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter, const char* defaultTitle = "");
		static std::string SaveFile(const char* filter, const char* defaultTitle = "");
	};

}
