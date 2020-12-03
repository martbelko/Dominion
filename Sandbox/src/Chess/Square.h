#pragma once

#include "Chessman.h"

#include <glm/glm.hpp>

#include <array>

class Square
{
public:
	Square() = default;
	Square(const glm::ivec2& offset, const glm::vec4& color)
		: m_Offset(offset), m_Color(color) {}
	Square(int xOffset, int yOffset, const glm::vec4& color)
		: m_Offset({ xOffset, yOffset }), m_Color(color) {}

	const glm::ivec2& GetOffset() const { return m_Offset; }
	const glm::vec4& GetColor() const { return m_Color; }
private:
	glm::ivec2 m_Offset;
	glm::vec4 m_Color;
};
