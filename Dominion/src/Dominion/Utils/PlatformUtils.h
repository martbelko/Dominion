#pragma once

#include <filesystem>

namespace Dominion {

	class FileDialogs
	{
	public:
		// These return empty strings if cancelled
		static std::filesystem::path OpenFile(const char* filter);
		static std::filesystem::path SaveFile(const char* filter);
	};

}
