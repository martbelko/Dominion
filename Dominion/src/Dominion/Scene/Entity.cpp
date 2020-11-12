#include "dmpch.h"
#include "Entity.h"

namespace Dominion {

	Entity::Entity(entt::entity entityHandle, Scene* scene)
		: m_EntityHandle(entityHandle), m_Scene(scene)
	{

	}

	bool Entity::operator==(const Entity& other) const
	{
		return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
	}

	bool Entity::operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

}
