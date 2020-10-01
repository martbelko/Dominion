#pragma once

#include "Dominion/Renderer/Bindable.h"

namespace Dominion {

	enum class DataType
	{
		Float,
		Float2,
		Float3,
		Float4,
		Mat2,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static unsigned int DataTypeSize(DataType type)
	{
		switch (type)
		{
			case Dominion::DataType::Float:  return sizeof(float);
			case Dominion::DataType::Float2: return 2 * sizeof(float);
			case Dominion::DataType::Float3: return 3 * sizeof(float);
			case Dominion::DataType::Float4: return 4 * sizeof(float);
			case Dominion::DataType::Mat2:   return 2 * 2 * sizeof(float);
			case Dominion::DataType::Mat3:   return 3 * 3 * sizeof(float);
			case Dominion::DataType::Mat4:   return 4 * 4 * sizeof(float);
			case Dominion::DataType::Int:    return sizeof(int);
			case Dominion::DataType::Int2:   return 2 * sizeof(int);
			case Dominion::DataType::Int3:   return 3 * sizeof(int);
			case Dominion::DataType::Int4:   return 4 * sizeof(int);
			case Dominion::DataType::Bool:   return sizeof(bool);
		}

		DM_CORE_ASSERT(false, "Unknown DataType!");
		return 0;
	}

	struct InputLayoutElement
	{
		const char* name;
		DataType type;
		unsigned int size;
		unsigned int offset = 0;

		InputLayoutElement(const char* name, DataType type)
			: name(name), type(type), size(DataTypeSize(type)) {}
	};

	class InputLayout : public Bindable
	{
	public:
		static std::shared_ptr<InputLayout> Create(const std::initializer_list<InputLayoutElement>& elements);
	};

}
