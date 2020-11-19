#include "dmpch.h"
#include "SceneSerializer.h"

#include "Dominion/Core/Filesystem.h"
#include "Dominion/Scene/Entity.h"
#include "Dominion/Scene/Components.h"
#include "Dominion/Scene/SceneCamera.h"
#include "Dominion/Renderer/Texture.h"

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);

			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);

			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};

}

namespace Dominion {

	static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec3)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec3.x << vec3.y << vec3.z << YAML::EndSeq;
		return out;
	}

	static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec4)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec4.x << vec4.y << vec4.z << vec4.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << "12837192831273";

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			std::string& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			TransformComponent& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << tc.Position;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			CameraComponent& cc = entity.GetComponent<CameraComponent>();
			SceneCamera& cam = cc.Cam;

			out << YAML::Key << "Camera";
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << static_cast<int>(cam.GetProjectionType());

			out << YAML::Key << "PerspectiveFOV" << YAML::Value << cam.GetPerspectiveFOV();
			out << YAML::Key << "PerspectiveNearClip" << YAML::Value << cam.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFarClip" << YAML::Value << cam.GetPerspectiveFarClip();

			out << YAML::Key << "OrthographicSize" << YAML::Value << cam.GetOrthographicSize();
			out << YAML::Key << "OrthographicNearClip" << YAML::Value << cam.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFarClip" << YAML::Value << cam.GetOrthographicFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			SpriteRendererComponent& sc = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << sc.Color;
			out << YAML::Key << "Texture" << YAML::Value << (sc.Texture ? Filesystem::GetRelativePathFromFullPath(sc.Texture->GetPath()).data() : ".");
			out << YAML::Key << "TilingFactor" << YAML::Value << sc.TilingFactor;

			out << YAML::EndMap; // SpriteRendererComponent
		}

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap; // Scene
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->GetName();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](entt::entity entityID)
		{
			Entity entity(entityID, m_Scene.get());
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap; // Scene

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		DM_CORE_ASSERT(false, "Not implemented");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream fin(filepath);
		std::stringstream ss;
		ss << fin.rdbuf();

		YAML::Node data = YAML::Load(ss.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		DM_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		m_Scene->SetName(sceneName);

		YAML::Node entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t id = entity["Entity"].as<uint64_t>(); // TODO

				YAML::Node tagComponent = entity["TagComponent"];
				if (!tagComponent)
				{
					DM_CORE_ASSERT(false, "TagComponent is required for every entity!");
					return false;
				}

				std::string entityName = tagComponent["Tag"].as<std::string>();
				DM_CORE_TRACE("Deserializing entity with ID = {0}, name = {1}", id, entityName);
				Entity deserializedEntity = m_Scene->CreateEntity(entityName);

				YAML::Node transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transform component
					TransformComponent& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Position = transformComponent["Position"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				YAML::Node cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					CameraComponent& cc = deserializedEntity.AddComponent<CameraComponent>();
					SceneCamera& cam = cc.Cam;

					YAML::Node& cameraProps = cameraComponent["Camera"];

					cam.SetProjectionType(static_cast<SceneCamera::ProjectionType>(cameraProps["ProjectionType"].as<int>()));

					cam.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
					cam.SetPerspectiveNearClip(cameraProps["PerspectiveNearClip"].as<float>());
					cam.SetPerspectiveFarClip(cameraProps["PerspectiveFarClip"].as<float>());

					cam.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cam.SetOrthographicNearClip(cameraProps["OrthographicNearClip"].as<float>());
					cam.SetOrthographicFarClip(cameraProps["OrthographicFarClip"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				YAML::Node spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					SpriteRendererComponent& sc = deserializedEntity.AddComponent<SpriteRendererComponent>();
					sc.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					std::string texPath = spriteRendererComponent["Texture"].as<std::string>();
					if (texPath == ".")
						sc.Texture = nullptr;
					else
						sc.Texture = Texture2D::Create(texPath);
					sc.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		DM_CORE_ASSERT(false, "Not implemented");
		return false;
	}

}
