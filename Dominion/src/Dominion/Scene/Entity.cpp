#include "dmpch.h"
#include "Entity.h"

namespace Dominion {

	Entity::Entity(entt::entity entityHandle, Scene* scene)
		: m_EntityHandle(entityHandle), m_Scene(scene)
	{

	}

}
