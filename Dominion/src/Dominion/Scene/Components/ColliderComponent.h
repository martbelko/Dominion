#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Physics/Physics.h"

#include <glm/glm.hpp>

namespace Dominion {

	struct BoxCollider2DComponent
	{
	public:
		const glm::vec2& GetCenterOffset() const { return m_CenterOffset; }
		void SetCenterOffset(const glm::vec2& centerOffset)
		{
			glm::vec2 offset = centerOffset - m_CenterOffset;
			m_CenterOffset = centerOffset;
			physx::PxTransform transform = m_PhysicsActor->getGlobalPose();
			transform.p.x += offset.x;
			transform.p.y += offset.y;
			m_PhysicsActor->setGlobalPose(transform);
		}
	private:
		glm::vec2 m_CenterOffset{ 0.0f, 0.0f };
		physx::PxRigidActor* m_PhysicsActor = nullptr;

		friend class Scene;
	};

}
