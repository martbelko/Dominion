#include "dmpch.h"
#include "OpenGLInputLayout.h"

#include <glad/glad.h>

namespace Dominion {

	static unsigned int GetComponentCount(DataType type)
	{
		switch (type)
		{
			case Dominion::DataType::Float:  return 1;
			case Dominion::DataType::Float2: return 2;
			case Dominion::DataType::Float3: return 3;
			case Dominion::DataType::Float4: return 4;
			case Dominion::DataType::Mat2:   return 2 * 2;
			case Dominion::DataType::Mat3:   return 3 * 3;
			case Dominion::DataType::Mat4:   return 4 * 4;
			case Dominion::DataType::Int:    return 1;
			case Dominion::DataType::Int2:   return 2;
			case Dominion::DataType::Int3:   return 3;
			case Dominion::DataType::Int4:   return 4;
			case Dominion::DataType::Bool:   return 1;
		}

		DM_CORE_ASSERT(false, "Unknown data type!");
		return 0;
	}

	static GLenum DataTypeToOpenGLType(DataType type)
	{
		switch (type)
		{
			case Dominion::DataType::Float:
			case Dominion::DataType::Float2:
			case Dominion::DataType::Float3:
			case Dominion::DataType::Float4:
			case Dominion::DataType::Mat2:
			case Dominion::DataType::Mat3:
			case Dominion::DataType::Mat4:
				return GL_FLOAT;
			case Dominion::DataType::Int:
			case Dominion::DataType::Int2:
			case Dominion::DataType::Int3:
			case Dominion::DataType::Int4:
				return GL_INT;
			case Dominion::DataType::Bool:
				return GL_BYTE;
		}

		DM_CORE_ASSERT(false, "Unknown data type!");
		return 0;
	}

	class VertexArray
	{
	public:
		VertexArray()
		{
			glCreateVertexArrays(1, &m_RendererID);
			glBindVertexArray(m_RendererID);
		}

		~VertexArray()
		{
			glDeleteVertexArrays(1, &m_RendererID);
		}
	private:
		unsigned int m_RendererID;
	};


	OpenGLInputLayoutElelent::OpenGLInputLayoutElelent(const InputLayoutElement& element)
		: type(DataTypeToOpenGLType(element.type)), componentCount(GetComponentCount(element.type)), offset(reinterpret_cast<void*>(element.offset)) {}


	OpenGLInputLayout::OpenGLInputLayout(const std::vector<InputLayoutElement>& elements, unsigned int stride)
		: m_Stride(stride)
	{
		static VertexArray sVertexArray;
		m_Elements.reserve(elements.size());
		for (const auto& element : elements)
		{
			m_Elements.emplace_back(element);
		}
	}

	OpenGLInputLayout::~OpenGLInputLayout()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLInputLayout::Bind()
	{
		unsigned int index = 0;
		for (auto& element : m_Elements)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.componentCount, element.type, GL_FALSE, m_Stride, (const void*)element.offset);

			++index;
		}
	}

}
