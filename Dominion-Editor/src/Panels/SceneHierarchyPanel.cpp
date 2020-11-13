#include "SceneHierarchyPanel.h"

#include <glm/gtc/type_ptr.hpp>

namespace Dominion {

	// TODO: Move to UI Utils
	void DrawVec3Control(const char* label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize{ lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.25f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

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
			m_SelectionContext = {};

		// Right-click on a blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		if (ImGui::Begin("Properties"))
		{
			if (m_SelectionContext)
			{
				DrawComponents(m_SelectionContext);

				if (ImGui::Button("Add Component"))
					ImGui::OpenPopup("AddComponent");

				if (ImGui::BeginPopup("AddComponent"))
				{
					if (ImGui::MenuItem("Camera"))
					{
						m_SelectionContext.AddComponent<CameraComponent>();
						ImGui::CloseCurrentPopup();
					}
					else if (ImGui::MenuItem("Sprite Renderer"))
					{
						m_SelectionContext.AddComponent<SpriteRendererComponent>();
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}
		}

		ImGui::End();

	}

	void SceneHierarhyPanel::DrawEntityNode(Entity entity)
	{
		std::string& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uint64_t>(static_cast<uint32_t>(entity))), flags, tag.c_str());
		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		bool markedForTermination = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Remove Entity"))
				markedForTermination = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags =  ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(9812524151), flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (markedForTermination)
		{
			if (m_SelectionContext == entity)
				m_SelectionContext = {};

			m_Context->DestroyEntity(entity);
		}
	}

	void SceneHierarhyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			std::string& tag = entity.GetComponent<TagComponent>().Tag;

			static char name[256];
			DM_CORE_ASSERT(sizeof(name) >= tag.size(), "Buffer Overflow");
			memset(name, 0, sizeof(name));
			strcpy(name, tag.c_str());
			if (ImGui::InputText("Tag", name, sizeof(name)))
			{
				tag = std::string(name);
			}
		}

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(TransformComponent).hash_code()), treeNodeFlags, "Transform"))
			{
				TransformComponent& tc = entity.GetComponent<TransformComponent>();
				DrawVec3Control("Position", tc.Position);
				glm::vec3 rotation = glm::degrees(tc.Rotation);
				DrawVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", tc.Scale, 1.0f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			bool open = ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(CameraComponent).hash_code()), treeNodeFlags, "Camera");

			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool markedForTermination = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					markedForTermination = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				CameraComponent& camComponent = entity.GetComponent<CameraComponent>();
				SceneCamera& cam = camComponent.Cam;

				ImGui::Checkbox("Primary", &camComponent.Primary);

				const char* projectionTypeString[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeString[static_cast<int>(cam.GetProjectionType())];
				if (ImGui::BeginCombo("Projection Type", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; ++i)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeString[i];
						if (ImGui::Selectable(projectionTypeString[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeString[i];
							cam.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (cam.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFOV = cam.GetPerspectiveFOV();
					if (ImGui::DragFloat("Vertical FOV", &verticalFOV))
						cam.SetPerspectiveFOV(verticalFOV);

					float perspectiveNear = cam.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						cam.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = cam.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						cam.SetPerspectiveFarClip(perspectiveFar);
				}
				else
				{
					float orthoSize = cam.GetOrthohraphicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						cam.SetOrthographicSize(orthoSize);

					float orthoNear = cam.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						cam.SetOrthographicNearClip(orthoNear);

					float orthoFar = cam.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						cam.SetOrthographicFarClip(orthoFar);
				}

				ImGui::Checkbox("Fixed Aspect Ratio", &camComponent.FixedAspectRation);

				ImGui::TreePop();
			}

			if (markedForTermination)
				entity.RemoveComponent<CameraComponent>();
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			bool open = ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(SpriteRendererComponent).hash_code()), treeNodeFlags, "Sprite Renderer");
			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool markedForTermination = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					markedForTermination = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				SpriteRendererComponent& sc = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(sc.Color));

				ImGui::TreePop();
			}

			if (markedForTermination)
				entity.RemoveComponent<SpriteRendererComponent>();
		}
	}

}
