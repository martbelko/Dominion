#include "dmpch.h"
#include "FontRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Dominion {

	FontRenderer::FontRenderer()
	{
		if (FT_Init_FreeType(&mFontLibrary))
		{
			DM_CORE_ASSERT(false, "Could not init FreeType Library");
		}
	}


	void FontRenderer::LoadFont(const std::filesystem::path& fontPath)
	{
		FT_Face face;
		if (FT_New_Face(mFontLibrary, fontPath.string().c_str(), 0, &face))
		{
			DM_CORE_ASSERT(false, "Failed to load font");
		}
	}

	void FontRenderer::RenderText()
	{
		// TODO: Implement
		DM_CORE_ASSERT(false, "Not implemented");
	}

}
