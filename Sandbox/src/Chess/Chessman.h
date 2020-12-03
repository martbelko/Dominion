#pragma once

#include <Dominion.h>

#include <array>
#include <vector>

enum Team { TEAM_WHITE = 0, TEAM_BLACK };
enum ChessmanType { FT_Pawn = 0, FT_Rook, FT_Knight, FT_Bishop, FT_Queen, FT_King };

// Forward declarations
class Square;
class Checkerboard;

class Chessman
{
public:
	Chessman() = default;
	Chessman(Square* square, Team team)
		: m_Square(square), m_Team(team) {}

	virtual ~Chessman() = default;

	const Square* GetSquare() const { return m_Square; }
	Team GetTeam() const { return m_Team; }
	bool IsInGame() const { return m_InGame; }

	void ThrowUp() { m_InGame = false; }

	virtual ChessmanType GetType() const { return FT_Pawn; }
	virtual std::vector<Square*> GetAvailableMoves() { return {}; }
protected:
	Square* m_Square;
	Team m_Team;
	bool m_InGame = true;
};

class Pawn : public Chessman
{
public:
	Pawn(Square* square, Team team, const Checkerboard* checkerboard)
		: Chessman(square, team), m_Checkerboard(checkerboard) {}
	virtual ~Pawn() = default;

	virtual ChessmanType GetType() const override { return FT_Pawn; }
	virtual std::vector<Square*> GetAvailableMoves() override;
private:
	const Checkerboard* m_Checkerboard;
};

class Rook : public Chessman
{
public:
	Rook(Square* square, Team team, const Checkerboard* checkerboard)
		: Chessman(square, team), m_Checkerboard(checkerboard) {}
	virtual ~Rook() = default;

	virtual ChessmanType GetType() const override { return FT_Rook; }
	virtual std::vector<Square*> GetAvailableMoves() override;
private:
	const Checkerboard* m_Checkerboard;
};

class Knight : public Chessman
{
public:
	Knight(Square* square, Team team, const Checkerboard* checkerboard)
		: Chessman(square, team), m_Checkerboard(checkerboard) {}
	virtual ~Knight() = default;

	virtual ChessmanType GetType() const override { return FT_Knight; }
	virtual std::vector<Square*> GetAvailableMoves() override;
private:
	const Checkerboard* m_Checkerboard;
};

class Bishop : public Chessman
{
public:
	Bishop(Square* square, Team team, const Checkerboard* checkerboard)
		: Chessman(square, team), m_Checkerboard(checkerboard) {}
	virtual ~Bishop() = default;

	virtual ChessmanType GetType() const override { return FT_Bishop; }
	virtual std::vector<Square*> GetAvailableMoves() override;
private:
	const Checkerboard* m_Checkerboard;
};

class Queen : public Chessman
{
public:
	Queen(Square* square, Team team, const Checkerboard* checkerboard)
		: Chessman(square, team), m_Checkerboard(checkerboard) {}
	virtual ~Queen() = default;

	virtual ChessmanType GetType() const override { return FT_Queen; }
	virtual std::vector<Square*> GetAvailableMoves() override;
private:
	const Checkerboard* m_Checkerboard;
};

class King : public Chessman
{
public:
	King(Square* square, Team team, const Checkerboard* checkerboard)
		: Chessman(square, team), m_Checkerboard(checkerboard) {}
	virtual ~King() = default;

	virtual ChessmanType GetType() const override { return FT_King; }
	virtual std::vector<Square*> GetAvailableMoves() override;
private:
	const Checkerboard* m_Checkerboard;
};

class ChessmanRenderer
{
public:
	static void Init();

	static void RenderChessman(const Chessman* chessman) { return RenderChessman(*chessman); }
	static void RenderChessman(const Chessman& chessman);
private:
	static std::array<std::array<Dominion::Ref<Dominion::Texture2D>, 2>, FT_King + 1> s_Textures;
};
