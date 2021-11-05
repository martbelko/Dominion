#include "dmpch.h"
#include "Entity.h"

namespace Dominion {

	Entity::Entity(entt::entity handle, Scene* scene)
		: mEntityHandle(handle), mScene(scene)
	{
	}

}
