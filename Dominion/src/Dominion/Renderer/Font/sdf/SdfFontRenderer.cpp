#include "dmpch.h"
#include "Dominion/Renderer/Font/sdf/SdfFontRenderer.h"

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

	static constexpr size_t QUAD_VERTEX_COUNT = 4;
	static constexpr size_t QUAD_INDEX_COUNT = 6;

	struct Renderer2DData
	{
		static constexpr uint32_t MAX_QUADS = 20000;
		static constexpr uint32_t MAX_QUAD_VERTICES = MAX_QUADS * QUAD_VERTEX_COUNT;
		static constexpr uint32_t MAX_QUAD_INDICES = MAX_QUADS * QUAD_INDEX_COUNT;
		static constexpr uint32_t MAX_QUAD_TEXTURE_SLOTS = 32; // TODO: RenderCaps

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> quadShader;
		Ref<Texture2D> whiteTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MAX_QUAD_TEXTURE_SLOTS> textureSlots;
		uint32_t textureSlotIndex = 1; // 0 = white texture

		glm::vec4 quadVertexPositions[QUAD_VERTEX_COUNT];

		SdfFontRenderer::Statistics stats;

		struct CameraData
		{
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
			glm::mat4 viewProjectionMatrix;
		};
		CameraData cameraBuffer;
		Ref<UniformBuffer> cameraUniformBuffer;
	};

	static Renderer2DData sData;

	void SdfFontRenderer::Init()
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
		sData.quadVertexBuffer = VertexBuffer::Create(sData.MAX_QUAD_VERTICES * sizeof(QuadVertex), nullptr, BufferUsage::DynamicDraw);
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
		Texture2DSpecification spec;
		spec.width = 1;
		spec.height = 1;
		spec.internalFormat = InternalFormat::RGBA8;
		spec.dataFormat = DataFormat::RGBA;
		spec.minFilter = TextureFilter::Linear;
		spec.magFilter = TextureFilter::Linear;
		spec.wrapS = Wrapping::Repeat;
		spec.wrapT = Wrapping::Repeat;

		sData.whiteTexture = Texture2D::Create(spec);
		uint32_t whiteTextureData = 0xffffffff;
		sData.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		// Set first texture slot to 0
		sData.textureSlots[0] = sData.whiteTexture;
		// Texture samplers
		int32_t samplers[sData.MAX_QUAD_TEXTURE_SLOTS];
		for (uint32_t i = 0; i < sData.MAX_QUAD_TEXTURE_SLOTS; ++i)
			samplers[i] = i;

		// ==== Shaders
		sData.quadShader = Shader::Create("assets/shaders/Renderer2DQuad.vert", "assets/shaders/Renderer2DQuad.frag");

		// ==== Camera uniform buffer
		sData.cameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void SdfFontRenderer::Shutdown()
	{
		DM_PROFILE_FUNCTION();

		delete[] sData.quadVertexBufferBase;
	}

	void SdfFontRenderer::BeginScene(const glm::mat4& projection)
	{
		DM_PROFILE_FUNCTION();

		sData.cameraBuffer.viewMatrix = glm::mat4(1.0f);
		sData.cameraBuffer.projectionMatrix = projection;
		sData.cameraBuffer.viewProjectionMatrix = projection;
		sData.cameraUniformBuffer->SetData(&sData.cameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void SdfFontRenderer::EndScene()
	{
		DM_PROFILE_FUNCTION();

		Flush();
	}

	void SdfFontRenderer::FlushQuads()
	{
		if (sData.quadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)sData.quadVertexBufferPtr - (uint8_t*)sData.quadVertexBufferBase);
			sData.quadVertexBuffer->SetData(sData.quadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < sData.textureSlotIndex; ++i)
				sData.textureSlots[i]->Bind(i);

			sData.quadShader->Bind();
			RenderCommand::DrawIndexed(Topology::Triangles, sData.quadVertexArray, sData.quadIndexCount);
			++sData.stats.drawCalls;
		}
	}
	void SdfFontRenderer::StartBatch()
	{
		StartQuadsBatch();
	}

	void SdfFontRenderer::Flush()
	{
		FlushQuads();
	}

	void SdfFontRenderer::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void SdfFontRenderer::StartQuadsBatch()
	{
		sData.quadIndexCount = 0;
		sData.quadVertexBufferPtr = sData.quadVertexBufferBase;
		sData.textureSlotIndex = 1;
	}

	void SdfFontRenderer::NextQuadsBatch()
	{
		FlushQuads();
		StartQuadsBatch();
	}

	void SdfFontRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void SdfFontRenderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void SdfFontRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void SdfFontRenderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void SdfFontRenderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		DM_PROFILE_FUNCTION();

		constexpr float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr float tilingFactor = 1.0f;

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

	void SdfFontRenderer::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
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

	void SdfFontRenderer::ResetStats()
	{
		memset(&sData.stats, 0, sizeof(Statistics));
	}

	SdfFontRenderer::Statistics SdfFontRenderer::GetStats()
	{
		return sData.stats;
	}

	uint32_t SdfFontRenderer::Statistics::GetTotalVertexCount() const
	{
		return quadCount * QUAD_VERTEX_COUNT;
	}

	uint32_t SdfFontRenderer::Statistics::GetTotalIndexCount() const
	{
		return quadCount * QUAD_INDEX_COUNT;
	}

}
