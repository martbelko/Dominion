#include "Checkerboard.h"

Checkerboard::Checkerboard()
{
	/* Initialize checkerboard squares */
	static constexpr glm::vec4 whiteColor = glm::vec4(0.92f, 0.74f, 0.2f, 1.0f);
	static constexpr glm::vec4 blackColor = glm::vec4(0.92f, 0.40f, 0.2f, 1.0f);

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if ((i + j) % 2 == 0)
				m_Squares[i * 8 + j] = Square(j, i, blackColor);
			else
				m_Squares[i * 8 + j] = Square(j, i, whiteColor);
		}
	}

	/* Initialize chessmen */
	// Pawns, team white
	for (int i = 0; i < 8 * 2 * 2; ++i)
	{
		m_Chessmen[i] = new Knight(&m_Squares[i % 8], TEAM_WHITE, this);
	}
}

Checkerboard::~Checkerboard()
{
	for (Chessman* chessman : m_Chessmen)
		delete chessman;
}
