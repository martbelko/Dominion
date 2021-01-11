#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Scene/Entity.h"

namespace Dominion {

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}

		virtual void OnCollisionStart(Entity entity) {};
		virtual void OnCollisionStay(Entity entity) {};
		virtual void OnCollisionEnd(Entity entity) {};
	private:
		Entity m_Entity;

		friend class Scene;
		friend struct NativeScriptComponent;
	};

}
