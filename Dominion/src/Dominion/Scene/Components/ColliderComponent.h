#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Physics/Physics.h"

#include <glm/glm.hpp>

namespace Dominion {

	struct BoxCollider2DComponent
	{
		glm::vec2 centerOffset{ 0.0f, 0.0f };
		physx::PxMaterial* physicsMaterial = Physics::GetPhysXPhysics()->createMaterial(1.0f, 1.0f, 0.0f);
		physx::PxRigidActor* physicsActor = nullptr;
	};

}
