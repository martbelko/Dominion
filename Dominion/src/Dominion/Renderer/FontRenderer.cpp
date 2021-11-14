#include "dmpch.h"
#include "FontRenderer.h"

#include "Dominion/Renderer/Camera.h"
#include "Dominion/Renderer/RenderCommand.h"

#include <ft2build.h>
#include FT_FREETYPE_H

// TODO: REMOVE!!!
#include <Glad/glad.h>

namespace Dominion {

	struct Character
	{
		uint32_t textureID;  // ID handle of the glyph texture
		glm::ivec2 size;       // Size of glyph
		glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
		uint32_t advance;    // Offset to advance to next glyph
	};

	std::unordered_map<char, Character> Characters;

	FT_Library mFontLibrary;

	FontRenderer::FontRenderer()
	{
		if (FT_Init_FreeType(&mFontLibrary))
		{
			DM_CORE_ASSERT(false, "Could not init FreeType Library");
		}
	}

	void FontRenderer::LoadFont(const std::filesystem::path& fontPath)
	{
		mShader = NativeShader("assets/shaders/Font.vert", "assets/shaders/Font.frag");

		FT_Face face;
		FT_New_Face(mFontLibrary, fontPath.string().c_str(), 0, &face);
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; ++c)
		{
			// Load character glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				DM_CORE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
				continue;
			}

			// generate texture
			uint32_t texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<uint32_t>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		FT_Done_Face(face);
		FT_Done_FreeType(mFontLibrary);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void FontRenderer::RenderText(const std::string& text, glm::vec2 position, float scale, const glm::vec3& color)
	{
		mShader.use();
		uint32_t x, y, width, height;
		RenderCommand::GetViewport(x, y, width, height);
		mShader.setMat4("projection", glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height)));
		mShader.setVec3("textColor", color);
		glBindVertexArray(VAO);

		// iterate through all characters
		for (char c : text)
		{
			Character ch = Characters[c];

			float xpos = position.x + ch.bearing.x * scale;
			float ypos = position.y - (ch.size.y - ch.bearing.y) * scale;

			float w = ch.size.x * scale;
			float h = ch.size.y * scale;
			// update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};

			// render glyph texture over quad
			glBindTextureUnit(0, ch.textureID);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			position.x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
