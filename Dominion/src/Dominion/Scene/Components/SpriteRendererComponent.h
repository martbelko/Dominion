#pragma once

#include "Dominion/Core/Base.h"

#include "Dominion/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace Dominion {

	struct SpriteRendererComponent
	{
		Ref<Texture2D> texture = nullptr;
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		F32 tilingFactor = 1.0f;

		bool flipX = false, flipY = false;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: color(color) {}
	};

}
