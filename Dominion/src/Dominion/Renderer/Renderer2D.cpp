#include "dmpch.h"
#include "Dominion/Renderer/Renderer2D.h"

#include "Dominion/Renderer/VertexArray.h"
#include "Dominion/Renderer/Shader.h"
#include "Dominion/Renderer/UniformBuffer.h"
#include "Dominion/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Dominion {

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texIndex;
		float tilingFactor;

		// Editor-only
		int entityID;
	};

	struct CircleVertex
	{
		glm::vec3 worldPosition;
		glm::vec3 localPosition;
		glm::vec4 color;
		float thickness;
		float fade;

		// Editor-only
		int entityID;
	};

	struct LineVertex
	{
		glm::vec3 position;
		glm::vec4 color;

		// Editor-only
		int entityID;
	};

	struct Renderer2DData
	{
		static constexpr uint32_t MAX_QUADS = 20000;
		static constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4;
		static constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;
		static constexpr uint32_t MAX_TEXTURE_SLOTS = 32; // TODO: RenderCaps

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> quadShader;
		Ref<Texture2D> whiteTexture;

		Ref<VertexArray> circleVertexArray;
		Ref<VertexBuffer> circleVertexBuffer;
		Ref<Shader> circleShader;

		Ref<VertexArray> lineVertexArray;
		Ref<VertexBuffer> lineVertexBuffer;
		Ref<Shader> lineShader;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		uint32_t circleIndexCount = 0;
		CircleVertex* circleVertexBufferBase = nullptr;
		CircleVertex* circleVertexBufferPtr = nullptr;

		uint32_t lineVertexCount = 0;
		LineVertex* lineVertexBufferBase = nullptr;
		LineVertex* lineVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS> textureSlots;
		uint32_t textureSlotIndex = 1; // 0 = white texture

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;

		struct CameraData
		{
			glm::mat4 viewProjection;
		};
		CameraData cameraBuffer;
		Ref<UniformBuffer> cameraUniformBuffer;
	};

	static Renderer2DData sData;

	void Renderer2D::Init()
	{
		DM_PROFILE_FUNCTION();

		// Quads
		sData.quadVertexArray = VertexArray::Create();

		sData.quadVertexBuffer = VertexBuffer::Create(sData.MAX_VERTICES * sizeof(QuadVertex));
		sData.quadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aPosition"     },
			{ ShaderDataType::Float4, "aColor"        },
			{ ShaderDataType::Float2, "aTexCoord"     },
			{ ShaderDataType::Float,  "aTexIndex"     },
			{ ShaderDataType::Float,  "aTilingFactor" },
			{ ShaderDataType::Int,    "aEntityID"     }
		});
		sData.quadVertexArray->AddVertexBuffer(sData.quadVertexBuffer);
		sData.quadVertexBufferBase = new QuadVertex[sData.MAX_VERTICES];
		uint32_t* quadIndices = new uint32_t[sData.MAX_INDICES];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < sData.MAX_INDICES; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, sData.MAX_INDICES);
		sData.quadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// Circles
		sData.circleVertexArray = VertexArray::Create();

		sData.circleVertexBuffer = VertexBuffer::Create(sData.MAX_VERTICES * sizeof(CircleVertex));
		sData.circleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aWorldPosition" },
			{ ShaderDataType::Float3, "aLocalPosition" },
			{ ShaderDataType::Float4, "aColor" },
			{ ShaderDataType::Float,  "aThickness" },
			{ ShaderDataType::Float,  "aFade" },
			{ ShaderDataType::Int,    "aEntityID" }
		});
		sData.circleVertexArray->AddVertexBuffer(sData.circleVertexBuffer);
		sData.circleVertexArray->SetIndexBuffer(quadIB); // Use quad IB
		sData.circleVertexBufferBase = new CircleVertex[sData.MAX_VERTICES];

		// Lines
		sData.lineVertexArray = VertexArray::Create();

		sData.lineVertexBuffer = VertexBuffer::Create(sData.MAX_VERTICES * sizeof(LineVertex));
		sData.lineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aPosition" },
			{ ShaderDataType::Float4, "aColor" },
			{ ShaderDataType::Int,    "aEntityID" }
		});
		sData.lineVertexArray->AddVertexBuffer(sData.lineVertexBuffer);
		sData.lineVertexBufferBase = new LineVertex[sData.MAX_VERTICES];

		sData.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		sData.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[sData.MAX_TEXTURE_SLOTS];
		for (uint32_t i = 0; i < sData.MAX_TEXTURE_SLOTS; ++i)
			samplers[i] = i;

		sData.quadShader = Shader::Create("assets/shaders/Renderer2DQuad.vert", "assets/shaders/Renderer2DQuad.frag");
		sData.circleShader = Shader::Create("assets/shaders/Renderer2DCircle.vert", "assets/shaders/Renderer2DCircle.frag");
		sData.lineShader = Shader::Create("assets/shaders/Renderer2DLine.vert", "assets/shaders/Renderer2DLine.frag");

		// Set first texture slot to 0
		sData.textureSlots[0] = sData.whiteTexture;

		sData.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		sData.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		sData.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		sData.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		sData.cameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		DM_PROFILE_FUNCTION();

		delete[] sData.quadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		DM_PROFILE_FUNCTION();

		sData.cameraBuffer.viewProjection = camera.GetViewProjectionMatrix();
		sData.cameraUniformBuffer->SetData(&sData.cameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		DM_PROFILE_FUNCTION();

		sData.cameraBuffer.viewProjection = camera.GetProjection() * glm::inverse(transform);
		sData.cameraUniformBuffer->SetData(&sData.cameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		DM_PROFILE_FUNCTION();

		sData.cameraBuffer.viewProjection = camera.GetViewProjection();
		sData.cameraUniformBuffer->SetData(&sData.cameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		DM_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::StartBatch()
	{
		sData.quadIndexCount = 0;
		sData.quadVertexBufferPtr = sData.quadVertexBufferBase;

		sData.circleIndexCount = 0;
		sData.circleVertexBufferPtr = sData.circleVertexBufferBase;

		sData.lineVertexCount = 0;
		sData.lineVertexBufferPtr = sData.lineVertexBufferBase;

		sData.textureSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		if (sData.quadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)sData.quadVertexBufferPtr - (uint8_t*)sData.quadVertexBufferBase);
			sData.quadVertexBuffer->SetData(sData.quadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < sData.textureSlotIndex; ++i)
				sData.textureSlots[i]->Bind(i);

			sData.quadShader->Bind();
			RenderCommand::DrawIndexed(sData.quadVertexArray, sData.quadIndexCount);
			++sData.stats.drawCalls;
		}

		if (sData.circleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)sData.circleVertexBufferPtr - (uint8_t*)sData.circleVertexBufferBase);
			sData.circleVertexBuffer->SetData(sData.circleVertexBufferBase, dataSize);

			sData.circleShader->Bind();
			RenderCommand::DrawIndexed(sData.circleVertexArray, sData.circleIndexCount);
			++sData.stats.drawCalls;
		}

		if (sData.lineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)sData.lineVertexBufferPtr - (uint8_t*)sData.lineVertexBufferBase);
			sData.lineVertexBuffer->SetData(sData.lineVertexBufferBase, dataSize);

			sData.lineShader->Bind();
			RenderCommand::Draw(sData.lineVertexArray, sData.lineVertexCount);
			++sData.stats.drawCalls;
		}
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
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

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		DM_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (sData.quadIndexCount >= Renderer2DData::MAX_INDICES)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; ++i)
		{
			sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[i];
			sData.quadVertexBufferPtr->color = color;
			sData.quadVertexBufferPtr->texCoord = textureCoords[i];
			sData.quadVertexBufferPtr->texIndex = textureIndex;
			sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
			sData.quadVertexBufferPtr->entityID = entityID;
			++sData.quadVertexBufferPtr;
		}

		sData.quadIndexCount += 6;

		++sData.stats.quadCount;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		DM_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (sData.quadIndexCount >= Renderer2DData::MAX_INDICES)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < sData.textureSlotIndex; ++i)
		{
			if (*sData.textureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (sData.textureSlotIndex >= Renderer2DData::MAX_TEXTURE_SLOTS)
				NextBatch();

			textureIndex = static_cast<float>(sData.textureSlotIndex);
			sData.textureSlots[sData.textureSlotIndex] = texture;
			++sData.textureSlotIndex;
		}

		for (size_t i = 0; i < quadVertexCount; ++i)
		{
			sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[i];
			sData.quadVertexBufferPtr->color = tintColor;
			sData.quadVertexBufferPtr->texCoord = textureCoords[i];
			sData.quadVertexBufferPtr->texIndex = textureIndex;
			sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
			sData.quadVertexBufferPtr->entityID = entityID;
			++sData.quadVertexBufferPtr;
		}

		sData.quadIndexCount += 6;

		++sData.stats.quadCount;
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		DM_PROFILE_FUNCTION();

		constexpr size_t circleVertexCount = 4;

		// TODO: Implement for circles
// 		if (sData.quadIndexCount >= Renderer2DData::MAX_INDICES)
// 			NextBatch();

		for (size_t i = 0; i < circleVertexCount; ++i)
		{
			sData.circleVertexBufferPtr->worldPosition = transform * sData.quadVertexPositions[i];
			sData.circleVertexBufferPtr->localPosition = sData.quadVertexPositions[i] * 2.0f;
			sData.circleVertexBufferPtr->color = color;
			sData.circleVertexBufferPtr->thickness = thickness;
			sData.circleVertexBufferPtr->fade = fade;
			sData.circleVertexBufferPtr->entityID = entityID;
			++sData.circleVertexBufferPtr;
		}

		sData.circleIndexCount += 6;

		++sData.stats.circleCount;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.texture)
			DrawQuad(transform, src.texture, src.tilingFactor, src.color, entityID);
		else
			DrawQuad(transform, src.color, entityID);
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		if (sData.lineVertexCount >= Renderer2DData::MAX_INDICES)
			NextBatch();

		sData.lineVertexBufferPtr->position = p0;
		sData.lineVertexBufferPtr->color = color;
		sData.lineVertexBufferPtr->entityID = entityID;
		++sData.lineVertexBufferPtr;

		sData.lineVertexBufferPtr->position = p1;
		sData.lineVertexBufferPtr->color = color;
		sData.lineVertexBufferPtr->entityID = entityID;
		++sData.lineVertexBufferPtr;

		sData.lineVertexCount += 2;

		++sData.stats.lineCount;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		glm::vec3 v0 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 v1 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 v2 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 v3 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);

		// TODO: Refactor with topology
		DrawLine(v0, v1, color, entityID);
		DrawLine(v1, v2, color, entityID);
		DrawLine(v2, v3, color, entityID);
		DrawLine(v3, v0, color, entityID);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec4 vertices[4];
		for (size_t i = 0; i < 4; ++i)
			vertices[i] = transform * sData.quadVertexPositions[i];

		// TODO: Refactor when added Topology
		DrawLine(vertices[0], vertices[1], color, entityID);
		DrawLine(vertices[1], vertices[2], color, entityID);
		DrawLine(vertices[2], vertices[3], color, entityID);
		DrawLine(vertices[3], vertices[0], color, entityID);
	}

	void Renderer2D::SetLineWidth(float lineWidth)
	{
		RenderCommand::SetLineWidth(lineWidth);
	}

	void Renderer2D::ResetStats()
	{
		memset(&sData.stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return sData.stats;
	}

}
