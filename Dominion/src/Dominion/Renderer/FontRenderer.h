#pragma once

#include "freetype/freetype.h"

#include <filesystem>

namespace Dominion {

	class FontRenderer
	{
	public:
		FontRenderer();
		~FontRenderer();
		// TODO: Rule of 5
	public:
		void LoadFont(const std::filesystem::path& fontPath);

		void RenderText();
	private:
		FT_Library mFontLibrary;
	};

}
