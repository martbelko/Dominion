#include "dmpch.h"
#include "FontRenderer.h"

#include "Dominion/Renderer/Camera.h"
#include "Dominion/Renderer/RenderCommand.h"
#include "Dominion/Renderer/Texture.h"
#include "Dominion/Renderer/VertexArray.h"
#include "Dominion/Renderer/Shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Dominion {

	struct Character
	{
		Ref<Texture2D> texture;
		glm::ivec2 size;
		glm::ivec2 bearing;
		uint32_t advance;
	};
	std::unordered_map<char, Character> Characters;

	FT_Library mFontLibrary;

	FontRenderer::FontRenderer()
	{
		if (FT_Init_FreeType(&mFontLibrary))
		{
			DM_CORE_ASSERT(false, "Could not init FreeType Library");
		}

		mFontRendererUniformBuffer = UniformBuffer::Create(sizeof(FontRendererData), 1);
	}

	FontRenderer::~FontRenderer()
	{
		if (sCount == 0)
			FT_Done_FreeType(mFontLibrary);
	}

	void FontRenderer::LoadFont(const std::filesystem::path& fontPath)
	{
		mShader = Shader::Create("assets/shaders/FontRenderer.vert", "assets/shaders/FontRenderer.frag");

		FT_Face face;
		FT_New_Face(mFontLibrary, fontPath.string().c_str(), 0, &face);
		FT_Set_Pixel_Sizes(face, 0, 48);

		// load first 128 characters of ASCII set
		for (uint32_t c = 0; c < 128; ++c)
		{
			// Load character glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				DM_CORE_ERROR("FreeType error: Failed to load Glyph {0}", c);
				continue;
			}

			// generate texture
			Texture2DSpecification spec;
			spec.width = face->glyph->bitmap.width;
			spec.height = face->glyph->bitmap.rows;
			spec.wrapS = Wrapping::ClampToEdge;
			spec.wrapT = Wrapping::ClampToEdge;
			spec.minFilter = TextureFilter::Linear;
			spec.magFilter = TextureFilter::Linear;
			spec.dataFormat = DataFormat::R;
			spec.internalFormat = InternalFormat::R8;

			Ref<Texture2D> texture = Texture2D::Create(spec);
			texture->SetData(face->glyph->bitmap.buffer, spec.height * spec.width);

			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<uint32_t>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}

		FT_Done_Face(face);

		Ref<VertexBuffer> vbo = VertexBuffer::Create(sizeof(float) * 6 * 7, nullptr, BufferUsage::DynamicDraw);
		vbo->SetLayout({
			{ ShaderDataType::Float2, "aPosition" },
			{ ShaderDataType::Float2, "aTexCoord" },
			{ ShaderDataType::Float3, "aColor" }
		});

		mVao = VertexArray::Create();
		mVao->AddVertexBuffer(vbo);
	}

	void FontRenderer::RenderText(const std::string& text, glm::vec2 position, float scale, const glm::vec3& color)
	{
		uint32_t x, y, width, height;
		RenderCommand::GetViewport(x, y, width, height);
		mFontRendererData.projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
		mFontRendererUniformBuffer->SetData(&mFontRendererData, sizeof(FontRendererData));
		mShader->Bind();
		mVao->Bind();

		for (char c : text)
		{
			Character ch = Characters[c];

			float xpos = position.x + ch.bearing.x * scale;
			float ypos = position.y - (ch.size.y - ch.bearing.y) * scale;

			float w = ch.size.x * scale;
			float h = ch.size.y * scale;
			// update VBO for each character
			float vertices[6][7] = {
				{ xpos,     ypos + h, 0.0f, 0.0f, color.r, color.g, color.b },
				{ xpos,     ypos,     0.0f, 1.0f, color.r, color.g, color.b },
				{ xpos + w, ypos,     1.0f, 1.0f, color.r, color.g, color.b },

				{ xpos,     ypos + h, 0.0f, 0.0f, color.r, color.g, color.b },
				{ xpos + w, ypos,     1.0f, 1.0f, color.r, color.g, color.b },
				{ xpos + w, ypos + h, 1.0f, 0.0f, color.r, color.g, color.b }
			};

			// render glyph texture over quad
			ch.texture->Bind(0);
			// update content of VBO memory
			mVao->GetVertexBuffers()[0]->SetData(vertices, sizeof(vertices));

			// render quad
			RenderCommand::Draw(Topology::Triangles, mVao, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			position.x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}
	}

	uint32_t FontRenderer::sCount = 0;

}
