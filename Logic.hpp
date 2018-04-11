#ifndef LOGIC_H
#define LOGIC_H

#include "Board.hpp"
#include "Menu.hpp"
#include <time.h>
#include <random>

class Logic
{
public:
	// pending : game is on !
	// gameover : user lost
	// win : user won
	enum class GameState {pending, lose, win} gameState;

	// Used to indicate wheter game should be 
	// restarted when current closes or not
	enum class EndGameState {restart, norestart} restartOnEnd;

	// user : user uses gui to play
	// random : computer reveal buttons randomly
	// console : user uses console to play
	enum class GameType {user, random, console};

	Logic(GameState gs, EndGameState egs):
	gameState(gs), restartOnEnd(egs){}

	// Returns true if game is over
	// False otherwise
	bool isGameOver() const;

	// Returns number of mines on board
	int getMinesNumber(Board &board) const;

	// If random is set, fills board with randomly
	// placed bombs in number of n.
	// If random is not set fills top row,
	// left and right diagonal with bom (n is ignored).
	void deployMines(int n, bool random, Board &board);

	// Deploys mines according to game difficulty
	void deployMinesByDiffculty(Menu::Difficulty diff, Board &board);

	// Reveals button and checks if game is won or lost
	void handleReveal(int x, int y, Board &board);

	// Negate flag state of given button
	static void flagButton(int xcoord, int ycoord, Board &board);

	// Sets logic to initial state
	void resetState();
};

#endif
