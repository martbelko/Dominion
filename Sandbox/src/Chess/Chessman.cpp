#include "Chessman.h"

#include "Square.h"
#include "Chessboard.h"

std::array<std::array<Dominion::Ref<Dominion::Texture2D>, 2>, FT_King + 1> ChessmanRenderer::s_Textures;

void ChessmanRenderer::Init()
{
	s_Textures[FT_Pawn] = { Dominion::Texture2D::Create("assets/Textures/PawnWhite.png"), Dominion::Texture2D::Create("assets/Textures/PawnBlack.png") };
	s_Textures[FT_Rook] = { Dominion::Texture2D::Create("assets/Textures/RookWhite.png"), Dominion::Texture2D::Create("assets/Textures/RookBlack.png") };
	s_Textures[FT_Knight] = { Dominion::Texture2D::Create("assets/Textures/KnightWhite.png"), Dominion::Texture2D::Create("assets/Textures/KnightBlack.png") };
	s_Textures[FT_Bishop] = { Dominion::Texture2D::Create("assets/Textures/BishopWhite.png"), Dominion::Texture2D::Create("assets/Textures/BishopBlack.png") };
	s_Textures[FT_Queen] = { Dominion::Texture2D::Create("assets/Textures/QueenWhite.png"), Dominion::Texture2D::Create("assets/Textures/QueenBlack.png") };
	s_Textures[FT_King] = { Dominion::Texture2D::Create("assets/Textures/KingWhite.png"), Dominion::Texture2D::Create("assets/Textures/KingBlack.png") };
}

std::vector<Square*> Pawn::GetAvailableMoves() const
{
	// TODO: Refactor
	std::vector<Square*> result;
	if (m_Team == TEAM_WHITE)
	{
		glm::vec2 offset = m_Square->GetOffset() + glm::ivec2(0, 1);
		Square* square = m_Chessboard->IsOffsetValid(offset) ? &m_Chessboard->At(offset) : nullptr;
		if (square && !square->GetStandingChessman())
		{
			result.emplace_back(square);
			if (!WasMoved())
			{
				offset = m_Square->GetOffset() + glm::ivec2(0, 2);
				square = m_Chessboard->IsOffsetValid(offset) ? &m_Chessboard->At(offset) : nullptr;
				if (square && !square->GetStandingChessman())
					result.emplace_back(square);
			}
		}

		offset = m_Square->GetOffset() + glm::ivec2(1, 1);
		square = m_Chessboard->IsOffsetValid(offset) ? &m_Chessboard->At(offset) : nullptr;
		if (square && square->GetStandingChessman() && square->GetStandingChessman()->GetTeam() != m_Team)
			result.emplace_back(square);

		offset = m_Square->GetOffset() + glm::ivec2(-1, 1);
		square = m_Chessboard->IsOffsetValid(offset) ? &m_Chessboard->At(offset) : nullptr;
		if (square && square->GetStandingChessman() && square->GetStandingChessman()->GetTeam() != m_Team)
			result.emplace_back(square);
	}
	else
	{
		glm::ivec2 offset = m_Square->GetOffset() + glm::ivec2(0, -1);
		Square* square = m_Chessboard->IsOffsetValid(offset) ? &m_Chessboard->At(offset) : nullptr;
		if (square && !square->GetStandingChessman())
		{
			result.emplace_back(square);
			if (!WasMoved())
			{
				offset = m_Square->GetOffset() + glm::ivec2(0, -2);
				square = m_Chessboard->IsOffsetValid(offset) ? &m_Chessboard->At(offset) : nullptr;
				if (square && !square->GetStandingChessman())
					result.emplace_back(square);
			}
		}

		offset = m_Square->GetOffset() + glm::ivec2(1, -1);
		square = m_Chessboard->IsOffsetValid(offset) ? &m_Chessboard->At(offset) : nullptr;
		if (square && square->GetStandingChessman() && square->GetStandingChessman()->GetTeam() != m_Team)
			result.emplace_back(square);

		offset = m_Square->GetOffset() + glm::ivec2(-1, -1);
		square = m_Chessboard->IsOffsetValid(offset) ? &m_Chessboard->At(offset) : nullptr;
		if (square && square->GetStandingChessman() && square->GetStandingChessman()->GetTeam() != m_Team)
			result.emplace_back(square);
	}

	return result;
}

std::vector<Square*> Rook::GetAvailableMoves() const
{
	std::vector<Square*> result;
	static constexpr std::array<glm::ivec2, 4> dirs{
		glm::ivec2(1, 0),
		glm::ivec2(0, 1),
		glm::ivec2(-1, 0),
		glm::ivec2(0, -1),
	};

	for (const glm::ivec2& dir : dirs)
	{
		glm::ivec2 offset = m_Square->GetOffset() + dir;
		Square* square = m_Chessboard->IsOffsetValid(offset) ? &m_Chessboard->At(offset) : nullptr;
		while (square)
		{
			const Chessman* chessman = square->GetStandingChessman();
			if (chessman)
			{
				if (chessman->GetTeam() != m_Team)
					result.push_back(square);
				break;
			}
			else
			{
				result.push_back(square);
			}

			offset += dir;
			square = m_Chessboard->IsOffsetValid(offset) ? &m_Chessboard->At(offset) : nullptr;
		}
	}

	return result;
}

std::vector<Square*> Knight::GetAvailableMoves() const
{
	// TODO: Implement
	return {};
}

std::vector<Square*> Bishop::GetAvailableMoves() const
{
	// TODO: Implement
	return {};
}

std::vector<Square*> Queen::GetAvailableMoves() const
{
	// TODO: Implement
	return {};
}

std::vector<Square*> King::GetAvailableMoves() const
{
	// TODO: Implement
	return {};
}

/* ChessmanRenderer */
void ChessmanRenderer::RenderChessman(const Chessman* chessman)
{
	const Square* square = chessman->GetSquare();
	ChessmanType t = chessman->GetType();
	Dominion::Renderer2D::DrawQuad(chessman->GetSquare()->GetOffset(), { 1.0f, 1.0f }, s_Textures[chessman->GetType()][chessman->GetTeam()]);
}
