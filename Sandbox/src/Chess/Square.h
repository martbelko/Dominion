#pragma once

#include "Chessman.h"

#include <glm/glm.hpp>

#include <array>

class Square
{
public:
	Square() = default;
	Square(const glm::ivec2& offset, Team teamColor , Chessman* chessman = nullptr)
		: m_Offset(offset), m_TeamColor(teamColor), m_Chessman(chessman) {}
	Square(int xOffset, int yOffset, Team teamColor, Chessman* chessman = nullptr)
		: m_Offset({ xOffset, yOffset }), m_TeamColor(teamColor), m_Chessman(chessman) {}

	const glm::ivec2& GetOffset() const { return m_Offset; }

	const Chessman* GetStandingChessman() const { return m_Chessman; }
	Chessman* GetStandingChessman() { return m_Chessman; }
	void SetStandingChessman(Chessman* chessman) { m_Chessman = chessman; }

	bool IsSelected() const { return m_Selected; }
	void Select() { m_Selected = true; }
	void Deselect() { m_Selected = false; }
private:
	glm::ivec2 m_Offset;
	Chessman* m_Chessman;
	Team m_TeamColor;
	bool m_Selected = false;

	friend class Chessboard;
};
