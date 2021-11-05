#include "ExampleLayer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer()
	: Layer("ExampleLayer"), mCameraController(1280.0f / 720.0f)
{
	mVertexArray = Dominion::VertexArray::Create();

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	Dominion::Ref<Dominion::VertexBuffer> vertexBuffer = Dominion::VertexBuffer::Create(vertices, sizeof(vertices));
	Dominion::BufferLayout layout = {
		{ Dominion::ShaderDataType::Float3, "a_Position" },
		{ Dominion::ShaderDataType::Float4, "a_Color" }
	};
	vertexBuffer->SetLayout(layout);
	mVertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	Dominion::Ref<Dominion::IndexBuffer> indexBuffer = Dominion::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	mVertexArray->SetIndexBuffer(indexBuffer);

	mSquareVA = Dominion::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	Dominion::Ref<Dominion::VertexBuffer> squareVB = Dominion::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ Dominion::ShaderDataType::Float3, "a_Position" },
		{ Dominion::ShaderDataType::Float2, "a_TexCoord" }
		});
	mSquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Dominion::Ref<Dominion::IndexBuffer> squareIB = Dominion::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	mSquareVA->SetIndexBuffer(squareIB);

	std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

	std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

	mShader = Dominion::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

	std::string flatColorShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

	std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

	mFlatColorShader = Dominion::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

	auto textureShader = mShaderLibrary.Load("assets/shaders/Texture.glsl");

	mTexture = Dominion::Texture2D::Create("assets/textures/Checkerboard.png");
	mChernoLogoTexture = Dominion::Texture2D::Create("assets/textures/ChernoLogo.png");

	textureShader->Bind();
	textureShader->SetInt("u_Texture", 0);
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(Dominion::Timestep ts)
{
	// Update
	mCameraController.OnUpdate(ts);

	// Render
	Dominion::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Dominion::RenderCommand::Clear();

	Dominion::Renderer::BeginScene(mCameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	mFlatColorShader->Bind();
	mFlatColorShader->SetFloat3("u_Color", mSquareColor);

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Dominion::Renderer::Submit(mFlatColorShader, mSquareVA, transform);
		}
	}

	auto textureShader = mShaderLibrary.Get("Texture");

	mTexture->Bind();
	Dominion::Renderer::Submit(textureShader, mSquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	mChernoLogoTexture->Bind();
	Dominion::Renderer::Submit(textureShader, mSquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	// Triangle
	// Dominion::Renderer::Submit(m_Shader, m_VertexArray);

	Dominion::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(mSquareColor));
	ImGui::End();
}

void ExampleLayer::OnEvent(Dominion::Event& e)
{
	mCameraController.OnEvent(e);
}
