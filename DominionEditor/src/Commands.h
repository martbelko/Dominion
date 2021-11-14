#pragma once

#include <Dominion/Core/UUID.h>
#include <Dominion/Scene/Entity.h>

#include <tuple>

namespace Dominion {

	enum class CommandType
	{
		Null = 0,
		AddEntity,
		AddComponent,
		DuplicateEntity
	};

	class Command
	{
	public:
		virtual ~Command() = default;
		// TODO: Rule of 5

		virtual void Do() = 0;
		virtual void Undo() = 0;

		virtual CommandType GetCommandType() const = 0;
	};

	class NullCommand final : public Command
	{
	public:
		virtual void Do() override { };
		virtual void Undo() override { };

		virtual CommandType GetCommandType() const { return CommandType::Null; }
	};

	class AddEntityCommand final : public Command
	{
	public:
		AddEntityCommand(const Ref<Scene>& scene, const std::string& name, UUID uuid = UUID());
		// TODO: Rule of 5

		virtual void Do() override;
		virtual void Undo() override;

		Entity GetCreatedEntity() const { return mEntity; }

		virtual CommandType GetCommandType() const { return CommandType::AddEntity; }
	private:
		Ref<Scene> mScene;
		UUID mUUID;
		std::string mName;

		Entity mEntity;
	};

	template<typename Component>
	class AddComponentCommand final : public Command
	{
	public:
		template<typename... Args>
		AddComponentCommand(Entity entity, Args&&... args)
			: mEntity(entity), mComponentCopy(args...)
		{
		}

		// TODO: Rule of 5

		virtual void Do() override
		{
			mEntity.AddComponent<Component>(mComponentCopy);
		}

		virtual void Undo() override
		{
			mComponentCopy = mEntity.GetComponent<Component>();
			mEntity.RemoveComponent<Component>();
		}

		virtual CommandType GetCommandType() const { return CommandType::AddComponent; }
	private:
		Entity mEntity;
		Component mComponentCopy;
	};

	class DuplicateEntityCommand : public Command
	{
	public:
		DuplicateEntityCommand(Entity srcEntity);
		// TODO: Rule of 5

		virtual void Do() override;
		virtual void Undo() override;

		Entity GetSourceEntity() const { return mSrcEntity; }
		Entity GetCreatedEntity() const { return mDstEntity; }

		virtual CommandType GetCommandType() const { return CommandType::DuplicateEntity; }
	private:
		Entity mSrcEntity;
		Entity mDstEntity;
	};

}
