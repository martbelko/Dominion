#pragma once

#include "Dominion/Core/Physics.h"

#include <glm/glm.hpp>

namespace Dominion {

	struct BoxCollider2DComponent
	{
		glm::vec2 center{ 0.0f, 0.0f };
		glm::vec2 scale{ 1.0f, 1.0f };
		physx::PxMaterial* physicsMaterial = Physics::GetPhysXPhysics()->createMaterial(0.5f, 0.5f, 0.1f);
		physx::PxRigidDynamic* physicsActor = nullptr;
		bool isTrigger = false;
	};

}
