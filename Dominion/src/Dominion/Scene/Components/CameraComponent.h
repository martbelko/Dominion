#pragma once

#include "Dominion/Core/Base.h"

#include "Dominion/Scene/SceneCamera.h"

#include <glm/glm.hpp>

namespace Dominion {

	enum class CameraClearFlag
	{
		SOLID_COLOR = 0,
		DEPTH_ONLY,
		DONT_CLEAR
	};

	struct CameraComponent
	{
		SceneCamera camera;

		bool primary;
		CameraClearFlag cameraClearFlag = CameraClearFlag::SOLID_COLOR;
		glm::vec4 backgroundColor{ 0.1f, 0.1f, 0.1f, 1.0f };

		I32F depth = 0;
	};

}
