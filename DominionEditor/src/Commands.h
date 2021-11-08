#pragma once

#include <Dominion/Core/UUID.h>
#include <Dominion/Scene/Entity.h>

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
		AddEntityCommand(Ref<Scene>& scene, const std::string& name, UUID uuid = UUID());
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

	class AddComponentCommand final : public Command
	{
	public:
		// TODO: Implement
		virtual void Do() override {}
		virtual void Undo() override {}

		virtual CommandType GetCommandType() const { return CommandType::AddComponent; }
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
