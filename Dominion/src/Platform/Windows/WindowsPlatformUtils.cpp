#include "dmpch.h"
#include "Dominion/Utils/PlatformUtils.h"

#include <commdlg.h>
#include <winuser.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Dominion/Core/Application.h"

#include <filesystem>

namespace Dominion {

	namespace Utils {

		static UINT DominionMessageBoxTypeToWinType(MessageBoxType type)
		{
			switch (type)
			{
				case Dominion::MessageBoxType::Info: return MB_ICONINFORMATION;
				case Dominion::MessageBoxType::Question: return MB_ICONQUESTION;
				case Dominion::MessageBoxType::Warning: return MB_ICONWARNING;
				case Dominion::MessageBoxType::Error: return MB_ICONERROR;
			}

			DM_CORE_ASSERT(false, "Unknown MessageBoxType!");
			return 0;
		}

	}

	std::filesystem::path FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return std::filesystem::path(ofn.lpstrFile);

		return std::filesystem::path("");

	}

	std::filesystem::path FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return std::filesystem::path(ofn.lpstrFile);

		return std::filesystem::path("");
	}

	void FileDialogs::MessageBox(MessageBoxType type, const char* title, const char* text)
	{
		HWND owner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		UINT winType = Utils::DominionMessageBoxTypeToWinType(type);
		MessageBoxA(owner, text, title, winType);
	}

}
