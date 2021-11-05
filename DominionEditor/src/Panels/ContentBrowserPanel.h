#pragma once

#include <filesystem>

#include "Dominion/Renderer/Texture.h"

namespace Dominion {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path mCurrentDirectory;

		Ref<Texture2D> mDirectoryIcon;
		Ref<Texture2D> mFileIcon;
	};

}
