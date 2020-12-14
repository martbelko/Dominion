#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Scene/SceneCamera.h"

namespace Dominion {

	struct CameraComponent
	{
		SceneCamera Cam;
		bool Primary = false;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

}
