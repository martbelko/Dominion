#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Scene/Entity.h"

namespace Dominion {

	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	private:
		Entity m_Entity;

		friend class Scene;
	};

}
