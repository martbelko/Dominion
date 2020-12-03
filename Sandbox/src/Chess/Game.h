#pragma once

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

class Checkerboard
{
public:
	Checkerboard()
	{
		static constexpr glm::vec4 whiteColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		static constexpr glm::vec4 blackolor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

		const glm::vec4* activeColor = &whiteColor;
		activeColor = &blackolor;

		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				if ((i + j) % 2 == 0)
					m_Squares[i * 10 + j] = Square(j, i, whiteColor);
				else
					m_Squares[i * 10 + j] = Square(j, i, blackolor);
			}
		}
	}

	const std::array<Square, 10 * 10>& GetSquares() const { return m_Squares; }
private:
	std::array<Square, 10 * 10> m_Squares;
};
