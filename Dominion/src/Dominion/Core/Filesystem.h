#pragma once

#include "Dominion/Core/Base.h"

#include <string>
#include <string_view>

namespace Dominion {

	class Filesystem
	{
	public:
		static void Init();

		static bool IsRelativePath(const std::string_view& path);
		static std::string_view GetRelativePathFromFullPath(const std::string_view& fullPath);
	private:
		static std::string s_FullPath;
	};

}
