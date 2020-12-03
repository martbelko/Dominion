#include "Chessman.h"

#include "Square.h"

std::array<std::array<Dominion::Ref<Dominion::Texture2D>, 2>, FT_King + 1> ChessmanRenderer::s_Textures;

void ChessmanRenderer::Init()
{
	s_Textures[FT_Pawn] = { Dominion::Texture2D::Create("assets/Textures/PawnWhite.png"), Dominion::Texture2D::Create("assets/Textures/PawnBlack.png") };
	s_Textures[FT_Rook] = { Dominion::Texture2D::Create("assets/Textures/unnamed.png"), Dominion::Texture2D::Create("assets/Textures/unnamed.png") };
	s_Textures[FT_Knight] = { Dominion::Texture2D::Create("assets/Textures/KnightWhite.png"), Dominion::Texture2D::Create("assets/Textures/unnamed.png") };
	s_Textures[FT_Bishop] = { Dominion::Texture2D::Create("assets/Textures/unnamed.png"), Dominion::Texture2D::Create("assets/Textures/unnamed.png") };
	s_Textures[FT_Queen] = { Dominion::Texture2D::Create("assets/Textures/unnamed.png"), Dominion::Texture2D::Create("assets/Textures/unnamed.png") };
	s_Textures[FT_King] = { Dominion::Texture2D::Create("assets/Textures/unnamed.png"), Dominion::Texture2D::Create("assets/Textures/unnamed.png") };
}

std::vector<Square*> Pawn::GetAvailableMoves()
{
	// TODO: Implement
	return {};
}

std::vector<Square*> Rook::GetAvailableMoves()
{
	// TODO: Implement
	return {};
}

std::vector<Square*> Knight::GetAvailableMoves()
{
	// TODO: Implement
	return {};
}

std::vector<Square*> Bishop::GetAvailableMoves()
{
	// TODO: Implement
	return {};
}

std::vector<Square*> Queen::GetAvailableMoves()
{
	// TODO: Implement
	return {};
}

std::vector<Square*> King::GetAvailableMoves()
{
	// TODO: Implement
	return {};
}

/* ChessmanRenderer */
void ChessmanRenderer::RenderChessman(const Chessman& chessman)
{
	glm::ivec2 o = chessman.GetSquare()->GetOffset();
	ChessmanType t = chessman.GetType();
	Dominion::Renderer2D::DrawQuad(chessman.GetSquare()->GetOffset() - glm::ivec2(4, 4), { 1.0f, 1.0f }, s_Textures[chessman.GetType()][chessman.GetTeam()],
		chessman.GetTeam() == TEAM_WHITE ? glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}
