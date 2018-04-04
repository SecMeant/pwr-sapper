#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "Board.hpp"
#include "Logic.hpp"

#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string>

#include <unistd.h>

#include <thread>
#include <random>

#include <SFML/Graphics.hpp>

constexpr int restartButtonFontSize = 26;
constexpr int gameStateMsgFontSize = 36;

#ifdef __unix__
constexpr const char * defaultFont 
	= "/usr/share/fonts/truetype/freefont/FreeMono.ttf";
#else
constexpr const char * defaultFont 
	= "C:\\Windows\\Fonts\\arial.ttf";
#endif

class GameWindow
{
private:
	// Handles game logic
	Logic logic;
	
	// Represents board of buttons
	Board board;

	// Holds size of game window
	int windowWidth;
	int windowHeight;

	// Used for random play
	// Randomly reveales buttons
	std::default_random_engine randomGen;

	// Holds path of directory that has assets
	const std::string assetsPath;

	// holds textures of numbers (0-8) that are rendered
	// to indicate mines count around revealed button
	std::array<sf::Texture, 9> numberTextures;
	sf::Texture flagTexture;

	sf::Font mainFont;

	sf::Text restartButton;

	// Draws win or lose when game ends
	sf::Text gameStateMsg;

	// Starts game
	void startGame();

	// Starts game in console
	void startGameConsole();

	// Randomly seletcs button to reveal
	void randomPlay();

	// Loads needed textures from assets directory
	// On success 0 is returned
	// Otherwise number of unsuccessfully loaded textures
	int loadAssets();

	// Loads font specified by passed string
	// to this->mainFont
	// If no errors occur 0 is returned
	// Non-zero otherwise
	int loadFonts(const std::string &fontname);

	// Sets stateMsg to appropriate value
	void setStateMsgToGameState();

	// Called when game is over to handle it
	void handleGameOver(sf::RenderWindow &wnd);

	// Waits for user to click on yes/no when asked 
	// for restart
	void handleRestart(sf::RenderWindow &wnd);

	/* SFML HELPER FUNCTIONS */
	// Draws grid of game board
	void drawBoard(sf::RenderWindow &wnd);

	// Draws horizontal lines of board grid
	void drawHorizontalGrid(sf::RenderWindow &wnd); 
	
	// Draws vertical lines of board grid
	void drawVerticalGrid(sf::RenderWindow &wnd);

	// Draws buttons on grid according to its state
	void drawBoardButtons(sf::RenderWindow &wnd);

	// Draws restart button on screen
	// All information about button is taken from
	// button class
	void drawRestartButton(sf::RenderWindow &wnd);

	// Draws you win or you lose string
	void drawGameStateMsg (sf::RenderWindow &wnd);

	// Sets text to middle of screen in width and
	// to 20% of height from top
	// Also makes font enough size to fit into screen
	void transformTextToWindowHeader(sf::Text &txt);

	// Wait for user to click on button (sf::Text)
	// As argument takes window to operate on and
	// vector of buttons to watch for.
	// Returns index in given vector of btn that has been clicked
	// if SIG KILL occured SIZE_MAX is returned
	// CAUTION: IF SIG KILL IS SENT PASSED WINDOW IS CLOSED
	size_t waitForButtonClick
	(const std::vector<sf::Text> &btns, sf::RenderWindow &wnd);

	// Very similar to overloaded version but watches only 1 button
	// Returns SIZE_MAX if SIG KILL occured
	// Other undefined value otherwise
	size_t waitForButtonClick
	(const sf::Text &btn, sf::RenderWindow &wnd);
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
	GameWindow(int width, int height);

	// Deallocates mem for fields
	~GameWindow();

	// Randomly deploys mines and starts game
	// Returns when game is over.
	// Returns wheter game should be restarted or not
	Logic::EndGameState initStartGame(int minesCount);

	void debug_display() const;
	void display() const;
};

#endif // GAMEWINDOW_H
