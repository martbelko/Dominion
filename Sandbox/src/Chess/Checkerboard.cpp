#include "Checkerboard.h"

Checkerboard::Checkerboard(const glm::vec4& whiteColor, const glm::vec4& blackColor)
{
	m_Colors[TEAM_WHITE] = whiteColor;
	m_Colors[TEAM_BLACK] = blackColor;

	/* Initialize checkerboard squares */
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if ((i + j) % 2 == 0)
				m_Squares[i * 8 + j] = Square(j, i, TEAM_BLACK);
			else
				m_Squares[i * 8 + j] = Square(j, i, TEAM_WHITE);
		}
	}

	/* Initialize chessmen */
	int chessIndex = 0, squareIndex = 0;
	for (; chessIndex < m_Chessmen.size(); ++squareIndex, chessIndex += 4)
	{
		// Pawns
		Square* squareWhite = &m_Squares[8 + squareIndex];
		Square* squareBlack = &m_Squares[8 * 8 - 8 - squareIndex - 1];
		Chessman* chessmanWhite = new Pawn(squareWhite, TEAM_WHITE, this);
		Chessman* chessmanBlack = new Pawn(squareBlack, TEAM_BLACK, this);
		m_Chessmen[chessIndex] = chessmanWhite;
		m_Chessmen[chessIndex + 1] = chessmanBlack;
		squareWhite->SetStandingChessman(chessmanWhite);
		squareBlack->SetStandingChessman(chessmanBlack);

		// Other
		squareWhite = &m_Squares[squareIndex];
		squareBlack = &m_Squares[8 * 8 - squareIndex - 1];
		switch (squareIndex)
		{
			case 0: case 7:
			{
				chessmanWhite = new Rook(squareWhite, TEAM_WHITE, this);
				chessmanBlack = new Rook(squareBlack, TEAM_BLACK, this);
				break;
			}
			case 1: case 6:
			{
				chessmanWhite = new Knight(squareWhite, TEAM_WHITE, this);
				chessmanBlack = new Knight(squareBlack, TEAM_BLACK, this);
				break;
			}
			case 2: case 5:
			{
				chessmanWhite = new Bishop(squareWhite, TEAM_WHITE, this);
				chessmanBlack = new Bishop(squareBlack, TEAM_BLACK, this);
				break;
			}
			case 3:
			{
				chessmanWhite = new Queen(squareWhite, TEAM_WHITE, this);
				chessmanBlack = new Queen(squareBlack, TEAM_BLACK, this);
				break;
			}
			case 4:
			{
				chessmanWhite = new King(squareWhite, TEAM_WHITE, this);
				chessmanBlack = new King(squareBlack, TEAM_BLACK, this);
				break;
			}
		}

		m_Chessmen[chessIndex + 2] = chessmanWhite;
		m_Chessmen[chessIndex + 3] = chessmanBlack;
		squareWhite->SetStandingChessman(chessmanWhite);
		squareBlack->SetStandingChessman(chessmanBlack);
	}
}

Checkerboard::~Checkerboard()
{
	for (Chessman* chessman : m_Chessmen)
		delete chessman;
}
