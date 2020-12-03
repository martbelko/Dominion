#pragma once

#include "Square.h"
#include "Chessman.h"

#include <glm/glm.hpp>

#include <array>

class Checkerboard
{
public:
	Checkerboard();
	~Checkerboard();

	const Square& At(int x, int y) const { return m_Squares[y * 8 + x]; }
	// Square& At(int x, int y) { return m_Squares[y * 8 + x]; }

	const std::array<Square, 8 * 8>& GetSquares() const { return m_Squares; }
	const std::array<Chessman*, 8 * 2 * 2>& GetChessmen() const { return m_Chessmen; }
private:
	std::array<Square, 8 * 8> m_Squares;
	std::array<Chessman*, 8 * 2 * 2> m_Chessmen;
};
