#include "dmpch.h"
#include "Renderer2D.h"

#include "Dominion/Renderer/RenderCommand.h"

#include "Dominion/Renderer/Pipeline.h"
#include "Dominion/Renderer/Shader.h"
#include "Dominion/Renderer/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Dominion {

	struct Renderer2DData
	{
		Ref<Pipeline> QuadPipeline;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		DM_PROFILE_FUNCTION();

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		s_Data.QuadPipeline = Dominion::Pipeline::Create(Dominion::VertexBuffer::Create(vertices, sizeof(vertices)), Dominion::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)), Dominion::InputLayout::Create(
			{
				{ "Position", Dominion::DataType::Float3 },
				{ "TexCoord", Dominion::DataType::Float2 }
			}
		));

		s_Data.TextureShader = Shader::Create("DefaultRenderer2DShader", "assets/Shaders/DefaultRenderer2DVS.glsl", "assets/Shaders/DefaultRenderer2DPS.glsl");

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));
	}

	void Renderer2D::Shutdown()
	{
		DM_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		DM_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		DM_PROFILE_FUNCTION();
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

		s_Data.TextureShader->SetMat4("u_Transform", transform);
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetInt("u_Texture", 0);
		s_Data.WhiteTexture->Bind(0);
		s_Data.QuadPipeline->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadPipeline);
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
