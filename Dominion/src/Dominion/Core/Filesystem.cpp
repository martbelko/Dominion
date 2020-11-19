#include "dmpch.h"
#include "Filesystem.h"

namespace Dominion {

	std::string Filesystem::s_FullPath;

	void Filesystem::Init()
	{
		char buff[FILENAME_MAX];
		_getcwd(buff, FILENAME_MAX);
		s_FullPath = buff;
		s_FullPath += "\\";
	}

	bool Filesystem::IsRelativePath(const std::string_view& path)
	{
		return path.find(s_FullPath) == std::string::npos;
	}

	std::string_view Filesystem::GetRelativePathFromFullPath(const std::string_view& fullPath)
	{
		if (fullPath.find(s_FullPath) != std::string::npos)
			return fullPath.substr(s_FullPath.size());

		DM_CORE_WARN("No relative path was found: '{0}'", fullPath);
		return fullPath;
	}

}
