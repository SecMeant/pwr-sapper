#ifndef BOARD_H
#define BOARD_H

#include "Field.hpp"
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <unistd.h>

#include <thread>
#include <random>

#include <SFML/Graphics.hpp>


class Board
{
public:
	// pending : game is on !
	// gameover : user lost
	// win : user won
	enum GameState {pending, lose, win};

	// user : user uses gui to play
	// random : computer reveal buttons randomly
	// console : user uses console to play
	enum GameType {user, random, console};

private:
	// Table of Width * Height fields
	Field *board;

	// Holds size of game window
	int windowWidth;
	int windowHeight;

	// Desribes span of mem that contains fields
	int boardWidth;
	int boardHeight;

	// Holds value of buttons that must be revealed to win
	int buttonsToReveal;

	// Returns ptr to field from board
	// described with XY coordinates
	// Returns nullptr if coordinates are out of bound
	Field* getFromBoard(int x, int y) const;

	GameState gameState;

	std::thread *UIthread;

	// Used for random play
	std::default_random_engine randomGen;

	// Holds path of directory that has assets
	const std::string assetsPath;

	// holds textures of numbers (0-8) that are rendered
	// to indicate mines count around revealed button
	std::array<sf::Texture, 9> numberTextures;
	sf::Texture flagTexture;

	// Starts game
	void startGame();

	// Starts game in console
	void startGameConsole();

	// Randomly seletcs button to reveal
	void randomPlay();

	// Returns number of mines on board
	int getMinesNumber() const;

	// Loads needed textures from assets directory
	// On success 0 is returned
	// Otherwise number of unsuccessfully loaded textures
	int loadAssets();

	// Negate flag state of given button
	void flagButton(int xcoord, int ycoord);

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

	inline Board::GameState getGameState();

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

	// same as reveal but first checks if button is flagged
	// if so does not reveals
	// Returns true if revealed button
	// false otherwise
	bool revealUnflagged(int x, int y);

	// Randomly deploys mines and starts game
	void initStartGame(Board::GameType gt, int minesCount);
};

#endif