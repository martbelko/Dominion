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

	static constexpr size_t QUAD_VERTEX_COUNT = 4;
	static constexpr size_t QUAD_INDEX_COUNT = 6;

	static constexpr size_t CIRCLE_VERTEX_COUNT = 4;
	static constexpr size_t CIRCLE_INDEX_COUNT = 6;

	static constexpr size_t LINE_VERTEX_COUNT = 2;

	struct Renderer2DData
	{
		static constexpr uint32_t MAX_QUADS = 20000;
		static constexpr uint32_t MAX_QUAD_VERTICES = MAX_QUADS * QUAD_VERTEX_COUNT;
		static constexpr uint32_t MAX_QUAD_INDICES = MAX_QUADS * QUAD_INDEX_COUNT;
		static constexpr uint32_t MAX_QUAD_TEXTURE_SLOTS = 32; // TODO: RenderCaps

		static constexpr uint32_t MAX_CIRCLES = 1000;
		static constexpr uint32_t MAX_CIRCLE_VERTICES = MAX_CIRCLES * CIRCLE_VERTEX_COUNT;
		static constexpr uint32_t MAX_CIRCLE_INDICES = MAX_CIRCLES * CIRCLE_INDEX_COUNT;

		static constexpr uint32_t MAX_LINES = 1000;
		static constexpr uint32_t MAX_LINE_VERTICES = MAX_LINES * LINE_VERTEX_COUNT;

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

		std::array<Ref<Texture2D>, MAX_QUAD_TEXTURE_SLOTS> textureSlots;
		uint32_t textureSlotIndex = 1; // 0 = white texture

		glm::vec4 quadVertexPositions[QUAD_VERTEX_COUNT];

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

		// ==== Quads ====
		// Init
		sData.quadVertexBufferBase = new QuadVertex[sData.MAX_QUAD_VERTICES];
		sData.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		sData.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		sData.quadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		sData.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
		// VertexBuffer
		sData.quadVertexBuffer = VertexBuffer::Create(sData.MAX_QUAD_VERTICES * sizeof(QuadVertex));
		sData.quadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aPosition"     },
			{ ShaderDataType::Float4, "aColor"        },
			{ ShaderDataType::Float2, "aTexCoord"     },
			{ ShaderDataType::Float,  "aTexIndex"     },
			{ ShaderDataType::Float,  "aTilingFactor" },
			{ ShaderDataType::Int,    "aEntityID"     }
		});
		// IndexBuffer
		uint32_t* quadIndices = new uint32_t[sData.MAX_QUAD_INDICES];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < sData.MAX_QUAD_INDICES; i += QUAD_INDEX_COUNT)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += QUAD_VERTEX_COUNT;
		}
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, sData.MAX_QUAD_INDICES);
		delete[] quadIndices;
		// VertexArray
		sData.quadVertexArray = VertexArray::Create();
		sData.quadVertexArray->AddVertexBuffer(sData.quadVertexBuffer);
		sData.quadVertexArray->SetIndexBuffer(quadIB);
		// Textures
		sData.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		sData.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		// Set first texture slot to 0
		sData.textureSlots[0] = sData.whiteTexture;
		// Texture samplers
		int32_t samplers[sData.MAX_QUAD_TEXTURE_SLOTS];
		for (uint32_t i = 0; i < sData.MAX_QUAD_TEXTURE_SLOTS; ++i)
			samplers[i] = i;

		// ==== Circles ====
		// Init
		sData.circleVertexBufferBase = new CircleVertex[sData.MAX_CIRCLE_VERTICES];
		// VertexBuffer
		sData.circleVertexBuffer = VertexBuffer::Create(sData.MAX_CIRCLE_VERTICES * sizeof(CircleVertex));
		sData.circleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aWorldPosition" },
			{ ShaderDataType::Float3, "aLocalPosition" },
			{ ShaderDataType::Float4, "aColor" },
			{ ShaderDataType::Float,  "aThickness" },
			{ ShaderDataType::Float,  "aFade" },
			{ ShaderDataType::Int,    "aEntityID" }
		});
		// IndexBuffer
		uint32_t* circleIndices = new uint32_t[sData.MAX_CIRCLE_INDICES];
		offset = 0;
		for (uint32_t i = 0; i < sData.MAX_CIRCLE_INDICES; i += CIRCLE_INDEX_COUNT)
		{
			circleIndices[i + 0] = offset + 0;
			circleIndices[i + 1] = offset + 1;
			circleIndices[i + 2] = offset + 2;

			circleIndices[i + 3] = offset + 2;
			circleIndices[i + 4] = offset + 3;
			circleIndices[i + 5] = offset + 0;

			offset += CIRCLE_VERTEX_COUNT;
		}
		Ref<IndexBuffer> circleIB = IndexBuffer::Create(circleIndices, sData.MAX_CIRCLE_INDICES); // Use quad indices
		delete[] circleIndices;
		// VertexArraz
		sData.circleVertexArray = VertexArray::Create();
		sData.circleVertexArray->AddVertexBuffer(sData.circleVertexBuffer);
		sData.circleVertexArray->SetIndexBuffer(circleIB);

		// ==== Lines ====
		// Init
		sData.lineVertexBufferBase = new LineVertex[sData.MAX_LINE_VERTICES];
		// VertexBuffer
		sData.lineVertexBuffer = VertexBuffer::Create(sData.MAX_LINE_VERTICES * sizeof(LineVertex));
		sData.lineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aPosition" },
			{ ShaderDataType::Float4, "aColor" },
			{ ShaderDataType::Int,    "aEntityID" }
		});
		// VertexArray
		sData.lineVertexArray = VertexArray::Create();
		sData.lineVertexArray->AddVertexBuffer(sData.lineVertexBuffer);

		// ==== Shaders
		sData.quadShader = Shader::Create("assets/shaders/Renderer2DQuad.vert", "assets/shaders/Renderer2DQuad.frag");
		sData.circleShader = Shader::Create("assets/shaders/Renderer2DCircle.vert", "assets/shaders/Renderer2DCircle.frag");
		sData.lineShader = Shader::Create("assets/shaders/Renderer2DLine.vert", "assets/shaders/Renderer2DLine.frag");

		// ==== Camera uniform buffer
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

	void Renderer2D::FlushQuads()
	{
		if (sData.quadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)sData.quadVertexBufferPtr - (uint8_t*)sData.quadVertexBufferBase);
			sData.quadVertexBuffer->SetData(sData.quadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < sData.textureSlotIndex; ++i)
				sData.textureSlots[i]->Bind(i);

			sData.quadShader->Bind();
			RenderCommand::DrawIndexed(Topology::TRIANGLES, sData.quadVertexArray, sData.quadIndexCount);
			++sData.stats.drawCalls;
		}
	}

	void Renderer2D::FlushCircles()
	{
		if (sData.circleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)sData.circleVertexBufferPtr - (uint8_t*)sData.circleVertexBufferBase);
			sData.circleVertexBuffer->SetData(sData.circleVertexBufferBase, dataSize);

			sData.circleShader->Bind();
			RenderCommand::DrawIndexed(Topology::TRIANGLES, sData.circleVertexArray, sData.circleIndexCount);
			++sData.stats.drawCalls;
		}
	}

	void Renderer2D::FlushLines()
	{
		if (sData.lineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)sData.lineVertexBufferPtr - (uint8_t*)sData.lineVertexBufferBase);
			sData.lineVertexBuffer->SetData(sData.lineVertexBufferBase, dataSize);

			sData.lineShader->Bind();
			RenderCommand::Draw(Topology::LINES, sData.lineVertexArray, sData.lineVertexCount);
			++sData.stats.drawCalls;
		}
	}

	void Renderer2D::StartBatch()
	{
		StartQuadsBatch();
		StartCirclesBatch();
		StartLinesBatch();
	}

	void Renderer2D::Flush()
	{
		FlushQuads();
		FlushCircles();
		FlushLines();
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::StartQuadsBatch()
	{
		sData.quadIndexCount = 0;
		sData.quadVertexBufferPtr = sData.quadVertexBufferBase;
		sData.textureSlotIndex = 1;
	}

	void Renderer2D::StartCirclesBatch()
	{
		sData.circleIndexCount = 0;
		sData.circleVertexBufferPtr = sData.circleVertexBufferBase;
	}

	void Renderer2D::StartLinesBatch()
	{
		sData.lineVertexCount = 0;
		sData.lineVertexBufferPtr = sData.lineVertexBufferBase;
	}

	void Renderer2D::NextQuadsBatch()
	{
		FlushQuads();
		StartQuadsBatch();
	}

	void Renderer2D::NextCirclesBatch()
	{
		FlushCircles();
		StartCirclesBatch();
	}

	void Renderer2D::NextLinesBatch()
	{
		FlushLines();
		StartLinesBatch();
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

		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (sData.quadIndexCount >= Renderer2DData::MAX_QUAD_INDICES)
			NextQuadsBatch();

		for (size_t i = 0; i < QUAD_VERTEX_COUNT; ++i)
		{
			sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[i];
			sData.quadVertexBufferPtr->color = color;
			sData.quadVertexBufferPtr->texCoord = textureCoords[i];
			sData.quadVertexBufferPtr->texIndex = textureIndex;
			sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
			sData.quadVertexBufferPtr->entityID = entityID;
			++sData.quadVertexBufferPtr;
		}

		sData.quadIndexCount += QUAD_INDEX_COUNT;

		++sData.stats.quadCount;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		DM_PROFILE_FUNCTION();

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (sData.quadIndexCount >= Renderer2DData::MAX_QUAD_INDICES)
			NextQuadsBatch();

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
			if (sData.textureSlotIndex >= Renderer2DData::MAX_QUAD_TEXTURE_SLOTS)
				NextQuadsBatch();

			textureIndex = static_cast<float>(sData.textureSlotIndex);
			sData.textureSlots[sData.textureSlotIndex] = texture;
			++sData.textureSlotIndex;
		}

		for (size_t i = 0; i < QUAD_VERTEX_COUNT; ++i)
		{
			sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[i];
			sData.quadVertexBufferPtr->color = tintColor;
			sData.quadVertexBufferPtr->texCoord = textureCoords[i];
			sData.quadVertexBufferPtr->texIndex = textureIndex;
			sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
			sData.quadVertexBufferPtr->entityID = entityID;
			++sData.quadVertexBufferPtr;
		}

		sData.quadIndexCount += QUAD_INDEX_COUNT;

		++sData.stats.quadCount;
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

	void Renderer2D::DrawCircle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		DrawCircle(glm::vec3(position, 0.0), size, color, thickness, fade, entityID);
	}

	void Renderer2D::DrawCircle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		DM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		DrawCircle(transform, color, thickness, fade, entityID);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		DM_PROFILE_FUNCTION();

		if (sData.circleIndexCount >= Renderer2DData::MAX_CIRCLE_INDICES)
			NextCirclesBatch();

		for (size_t i = 0; i < CIRCLE_VERTEX_COUNT; ++i)
		{
			sData.circleVertexBufferPtr->worldPosition = transform * sData.quadVertexPositions[i];
			sData.circleVertexBufferPtr->localPosition = sData.quadVertexPositions[i] * 2.0f;
			sData.circleVertexBufferPtr->color = color;
			sData.circleVertexBufferPtr->thickness = thickness;
			sData.circleVertexBufferPtr->fade = fade;
			sData.circleVertexBufferPtr->entityID = entityID;
			++sData.circleVertexBufferPtr;
		}

		sData.circleIndexCount += CIRCLE_INDEX_COUNT;

		++sData.stats.circleCount;
	}

	void Renderer2D::DrawLine(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color, int entityID)
	{
		DrawLine(glm::vec3(p0, 0.0f), glm::vec3(p1, 0.0f), color, entityID);
	}

	void Renderer2D::DrawLine(const glm::vec2& p0, const glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		DrawLine(glm::vec3(p0, 0.0f), p1, color, entityID);
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec2& p1, const glm::vec4& color, int entityID)
	{
		DrawLine(p0, glm::vec3(p1, 0.0f), color, entityID);
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		if (sData.lineVertexCount >= Renderer2DData::MAX_LINE_VERTICES)
			NextLinesBatch();

		sData.lineVertexBufferPtr->position = p0;
		sData.lineVertexBufferPtr->color = color;
		sData.lineVertexBufferPtr->entityID = entityID;
		++sData.lineVertexBufferPtr;

		sData.lineVertexBufferPtr->position = p1;
		sData.lineVertexBufferPtr->color = color;
		sData.lineVertexBufferPtr->entityID = entityID;
		++sData.lineVertexBufferPtr;

		sData.lineVertexCount += LINE_VERTEX_COUNT;

		++sData.stats.lineCount;
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		DrawRect(glm::vec3(position, 0.0f), size, color, entityID);
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		DM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		DrawRect(transform, color, entityID);
	}

	void Renderer2D::DrawRotatedRect(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, int entityID)
	{
		DrawRotatedRect(glm::vec3(position, 0.0f), size, rotation, color, entityID);
	}

	void Renderer2D::DrawRotatedRect(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color, int entityID)
	{
		DM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawRect(transform, color, entityID);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 vertices[4];
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
