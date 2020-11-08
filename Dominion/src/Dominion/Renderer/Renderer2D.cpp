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
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
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

		s_Data.TextureShader = Shader::Create("DefaultRenderer2DShader", "assets/Shaders/TextureVS.glsl", "assets/Shaders/TexturePS.glsl");
	}

	void Renderer2D::Shutdown()
	{

	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		s_Data.TextureShader->SetMat4("u_Transform", transform);
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.QuadPipeline->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadPipeline);
	}

}
