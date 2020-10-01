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

	OpenGLInputLayout::OpenGLInputLayout(const std::vector<InputLayoutElement>& elements, unsigned int stride)
	{
		glCreateVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);

		unsigned int index = 0;
		for (auto& element : elements)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, GetComponentCount(element.type), DataTypeToOpenGLType(element.type), GL_FALSE, stride, (const void*)element.offset);

			++index;
		}
	}

	OpenGLInputLayout::~OpenGLInputLayout()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLInputLayout::Bind()
	{
		glBindVertexArray(m_RendererID);
	}

}
