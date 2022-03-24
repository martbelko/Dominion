#pragma once

#include <filesystem>

namespace Dominion {

	enum class MessageBoxType
	{
		Info = 0, Question, Warning, Error
	};

	class FileDialogs
	{
	public:
		// These return empty strings if cancelled
		static std::filesystem::path OpenFile(const char* filter);
		static std::filesystem::path SaveFile(const char* filter);

		// TODO: Move to another class!
		static void MessageBox(MessageBoxType type, const char* title, const char* text);
	};

}
