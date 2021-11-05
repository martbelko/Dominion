#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Scene/Scene.h"
#include "Dominion/Scene/Entity.h"

namespace Dominion {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return mSelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> mContext;
		Entity mSelectionContext;
	};

}
