#pragma once

#include "Dominion/Core/Base.h"
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

	static uint32_t DataTypeSize(DataType type)
	{
		switch (type)
		{
			case DataType::Float:  return sizeof(float);
			case DataType::Float2: return 2 * sizeof(float);
			case DataType::Float3: return 3 * sizeof(float);
			case DataType::Float4: return 4 * sizeof(float);
			case DataType::Mat2:   return 2 * 2 * sizeof(float);
			case DataType::Mat3:   return 3 * 3 * sizeof(float);
			case DataType::Mat4:   return 4 * 4 * sizeof(float);
			case DataType::Int:    return sizeof(int);
			case DataType::Int2:   return 2 * sizeof(int);
			case DataType::Int3:   return 3 * sizeof(int);
			case DataType::Int4:   return 4 * sizeof(int);
			case DataType::Bool:   return sizeof(bool);
		}

		DM_CORE_ASSERT(false, "Unknown DataType!");
		return 0;
	}

	struct InputLayoutElement
	{
		std::string name;
		DataType type;
		uint32_t size;
		uint32_t offset = 0;

		InputLayoutElement(const char* name, DataType type)
			: name(name), type(type), size(DataTypeSize(type)) {}
		InputLayoutElement(const std::string_view& name, DataType type)
			: name(name), type(type), size(DataTypeSize(type)) {}
	};

	class InputLayout : public Bindable
	{
	public:
		static Ref<InputLayout> Create(const std::initializer_list<InputLayoutElement>& elements);

		virtual uint32_t GetStride() const = 0;
	};

}
