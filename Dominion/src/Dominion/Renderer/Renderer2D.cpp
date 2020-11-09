#include "dmpch.h"
#include "Renderer2D.h"

#include "Dominion/Renderer/RenderCommand.h"

#include "Dominion/Renderer/Pipeline.h"
#include "Dominion/Renderer/Shader.h"
#include "Dominion/Renderer/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Dominion {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};


	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Pipeline> QuadPipeline;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		glm::vec4 QuadVertexPositions[4];
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		DM_PROFILE_FUNCTION();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);

		delete[] quadIndices;

		s_Data.QuadPipeline = Dominion::Pipeline::Create(s_Data.QuadVertexBuffer, quadIB, Dominion::InputLayout::Create(
			{
				{ "Position", Dominion::DataType::Float3 },
				{ "Color", Dominion::DataType::Float4 },
				{ "TexCoord", Dominion::DataType::Float2 }
			}
		));

		s_Data.TextureShader = Shader::Create("DefaultRenderer2DShader", "assets/Shaders/DefaultRenderer2DVS.glsl", "assets/Shaders/DefaultRenderer2DPS.glsl");

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		DM_PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		DM_PROFILE_FUNCTION();

		s_Data.QuadPipeline->Bind();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

	}

	void Renderer2D::EndScene()
	{
		DM_PROFILE_FUNCTION();

		uint32_t dataSize = reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_Data.QuadPipeline, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor)
	{
		DM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		DM_PROFILE_FUNCTION();

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (int i = 0; i < 4; ++i)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor)
	{
		DM_PROFILE_FUNCTION();

		s_Data.TextureShader->SetMat4("u_Transform", transform);
		s_Data.TextureShader->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		s_Data.TextureShader->SetInt("u_Texture", 0);
		texture->Bind(0);
		s_Data.QuadPipeline->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadPipeline);
	}

}
