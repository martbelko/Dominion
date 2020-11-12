#pragma once

#include <Dominion.h>

namespace Dominion {

	class Scene;

	class SceneHierarhyPanel
	{
	public:
		SceneHierarhyPanel() = default;
		SceneHierarhyPanel(Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}
