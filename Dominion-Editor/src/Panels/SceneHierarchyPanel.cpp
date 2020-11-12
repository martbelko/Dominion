#include "SceneHierarchyPanel.h"

#include <glm/gtc/type_ptr.hpp>

namespace Dominion {

	SceneHierarhyPanel::SceneHierarhyPanel(Ref<Scene>& context)
		: m_Context(context)
	{

	}

	void SceneHierarhyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarhyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity = Entity(entityID, m_Context.get());
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
		}

		ImGui::End();

		if (ImGui::Begin("Properties"))
		{
			if (m_SelectionContext)
				DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarhyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uint64_t>(static_cast<uint32_t>(entity))), flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags =  ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(9812524151), flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}

	void SceneHierarhyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			static char name[256];
			DM_CORE_ASSERT(sizeof(name) >= tag.size(), "Buffer Overflow");
			memset(name, 0, sizeof(name));
			strcpy(name, tag.c_str());
			DM_TRACE(ImGui::IsItemActive());
			if (ImGui::InputText("Tag", name, sizeof(name)))
			{
				tag = std::string(name);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(TransformComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}
	}

}
