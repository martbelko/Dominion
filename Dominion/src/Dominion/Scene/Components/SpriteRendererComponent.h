#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace Dominion {

	struct SpriteRendererComponent
	{
		Ref<Texture2D> Texture = nullptr;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		F32 TilingFactor = 1.0f;

		bool FlipX = false, FlipY = false;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4 & color)
			: Color(color) {}
	};

}
