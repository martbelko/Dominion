#pragma once

#include "Square.h"
#include "Chessman.h"

#include <glm/glm.hpp>

#include <array>

class Checkerboard
{
public:
	Checkerboard(const glm::vec4& whiteColor = glm::vec4(0.92f, 0.74f, 0.2f, 1.0f), const glm::vec4& blackColor = glm::vec4(0.92f, 0.40f, 0.2f, 1.0f));
	~Checkerboard();

	const Square& At(const glm::ivec2& offset) const { return m_Squares[offset.y * 8 + offset.x]; }
	Square& At(const glm::ivec2& offset) { return m_Squares[offset.y * 8 + offset.x]; }
	const Square& At(int x, int y) const { return m_Squares[y * 8 + x]; }
	Square& At(int x, int y) { return m_Squares[y * 8 + x]; }

	void SetWhiteSquareColor(const glm::vec4& color) { m_Colors[TEAM_WHITE] = color; }
	void SetBlackSquareColor(const glm::vec4& color) { m_Colors[TEAM_BLACK] = color; }
	const glm::vec4& GetSquareColor(const Square* square) const { return m_Colors[square->m_TeamColor]; }
	const glm::vec4& GetSquareColor(const Square& square) const { return GetSquareColor(&square); }

	const std::array<Square, 8 * 8>& GetSquares() const { return m_Squares; }
	const std::array<Chessman*, 8 * 2 * 2>& GetChessmen() const { return m_Chessmen; }
private:
	std::array<Square, 8 * 8> m_Squares;
	std::array<Chessman*, 8 * 2 * 2> m_Chessmen;

	glm::vec4 m_Colors[2];
};
