#ifndef BOARD_H
#define BOARD_H

#include "Field.h"
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <unistd.h>

#include <thread>
#include <random>

#include <SFML/Graphics.hpp>

class Board
{
private:
	// Table of Width * Height fields
	Field *board;

	// Holds size of game window
	int windowWidth;
	int windowHeight;

	// Desribes span of mem that contains fields
	int boardWidth;
	int boardHeight;

	// Returns ptr to field from board
	// described with XY coordinates
	// Returns nullptr if coordinates are out of bound
	Field* getFromBoard(int x, int y) const;

	// True = GAMEOVER
	bool gameState;

	std::thread *UIthread;

	// Used for random play
	std::default_random_engine randomGen;

	// Starts game
	void startGame();

	// Starts game in console
	void startGameConsole();

	// Randomly seletcs button to reveal
	void randomPlay();

	/* SFML HELPER FUNCTIONS */
	// Draws grid of game board
	void drawBoard(sf::RenderWindow &wnd);

	// Draws horizontal lines of board grid
	void drawHorizontalGrid(sf::RenderWindow &wnd); 
	
	// Draws vertical lines of board grid
	void drawVerticalGrid(sf::RenderWindow &wnd);

	// Draws buttons on grid according to its state
	void drawBoardButtons(sf::RenderWindow &wnd);
public:
	// Offset of main board 
	// from top left corner of game window
	static constexpr float boardScreenXoffset = 75.0;
	static constexpr float boardScreenYoffset = 75.0;

	// Size of cells
	static constexpr float cellWidth = 35.0;
	static constexpr float cellHeight = 35.0;

	// allocates memory for fields in 
	// size of width*height*sizeof(Field)
	Board(int width, int height);

	// Deallocates mem for fields
	~Board();

	// If random is set, fills board with randomly
	// placed bombs in number of n.
	// If random is not set fills top row,
	// left and right diagonal with bom (n is ignored).
	void deployMines(int n, bool random);

	void debug_display() const;
	void display() const;
	
	// Changes field described by given XY coords to given values
	bool setField(int x, int y, bool mState, bool cState, bool fState);

	// Returns true if game is over
	// False otherwise
	bool isGameOver();

	// Returns true if field described by XY coords
	// contains bomb or not
	bool hasMine(int x, int y) const;

	// Returns number of mines around point
	// descibed as given XY coords
	int countMines(int x, int y) const;

	// Reveals filed descibed as given coords
	// Returns true if successufully revelad given field
	// False otherwise
	bool reveal(int x, int y);

	// Describes gametype
	enum GameType {user, random, console};

	// Randomly deploys mines and starts game
	void initStartGame(Board::GameType gt);
};

#endif
