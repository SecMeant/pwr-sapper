#include <sstream>
#include "GameWindow.hpp"

GameWindow::GameWindow(int width, int height)
:board(width, height),
 restartOnEnd(Logic::EndGameState::norestart),
 gameState(Logic::GameState::pending),
 randomGen(time(NULL)),
 assetsPath("./assets/")
{
	this->windowWidth = int(boardScreenXoffset*2 + width*cellWidth);
	this->windowHeight = int(boardScreenYoffset*2 + height*cellHeight);

	this->loadAssets();
	this->loadFonts(defaultFont);

	{ // deletes helper variables
		this->restartButton.setString("Restart");
		this->restartButton.setFont(mainFont);
		this->restartButton.setCharacterSize(restartButtonFontSize);
		this->restartButton.setFillColor({255,0,255});
		
		int restartButtonWidth = this->restartButton.getGlobalBounds().width;
		int restartButtonHeight = this->restartButton.getGlobalBounds().height;
		register int xoffRB = (this->windowWidth-restartButtonWidth)/2;
		register int yoffRB = this->windowHeight-restartButtonHeight-30;
		
		this->restartButton.setPosition(xoffRB,yoffRB);
	}

	{ // ditto
		this->gameStateMsg.setString("asdf");
		this->gameStateMsg.setFont(mainFont);
		this->gameStateMsg.setCharacterSize(gameStateMsgFontSize);
		this->gameStateMsg.setFillColor({0,255,0});

		int gameStateMsgWidth = this->gameStateMsg.getGlobalBounds().width;
		register int xoffRB = (this->windowWidth-gameStateMsgWidth)/2;
		register int yoffRB = 0;
		
		this->gameStateMsg.setPosition(xoffRB,yoffRB);
	}
}

GameWindow::~GameWindow()
{
}

void GameWindow::deployMines(int n, bool random)
{
	Field *f;

	if(random)
	{

		// Calculate buttons to reveal
		this->board.buttonsToReveal = this->board.width*this->board.height;
		this->board.buttonsToReveal -= n;

		for(int i = 0; i < n; i++)
		{
			int xcoord = int(this->randomGen() % this->board.width);
			int ycoord = int(this->randomGen() % this->board.height);

			f = this->board.get(xcoord, ycoord);
			// I know its slow ;]
			if(f->isMined())
			{
				i--;
				continue;
			}

			f->setMineState(true);
		}
		return;
	}
	
	for(int i = 0, j = 0; i < this->board.width; i++, j++)
	{
		// Negative from bottom diagonal slope
		f = this->board.get(this->board.width-1-i, this->board.height-1-i);
		if(f!=nullptr)
			f->setMineState(true);
		
		// Positive from bottom diagonal slope
		f = this->board.get(i, this->board.height-1-j);
		if(f!=nullptr)
			f->setMineState(true);

		// Top row
		f = this->board.get(i, 0);
		if(f!=nullptr)
			f->setMineState(true);
	}

	// Calculate buttons to reveal
	this->board.buttonsToReveal = this->board.width*this->board.height;
	this->board.buttonsToReveal -= this->getMinesNumber();
	printf("%i",this->board.buttonsToReveal);
}

void GameWindow::debug_display() const
{
	for(int j = 0; j < this->board.height; j++)
	{
		for(int i = 0; i < this->board.width; i++)
		{
			this->board.get(i, j)->info();
		}
		putchar('\n');
	}
}

void GameWindow::display() const
{
	Field *f;
	int minesCount;
	for(int j = 0; j < this->board.height; j++)
	{
		for(int i = 0; i < this->board.width; i++)
		{
			f = this->board.get(i,j);
			minesCount = this->board.countMines(i,j);
			putchar('[');

			if(f->isCovered())
			{
				if(f->isFlagged())
					putchar('?');
				else
					putchar('.');
			}
			else // revealed
			{
				if(f->isMined())
					putchar('X');
				else if(minesCount == 0)
					putchar(' ');
				else
					// Converts int to ascii digit
					putchar(static_cast<char>(minesCount+0x30));
			}

			putchar(']');
		}
		putchar('\n');
	}
}

bool GameWindow::isGameOver()
{
	if(this->gameState == Logic::GameState::pending)
		return false;
	return true;
}

Logic::GameState GameWindow::getGameState()
{
	return this->gameState;
}

Logic::EndGameState
GameWindow::initStartGame(int minesCount)
{
	this->deployMines(minesCount,true);
	this->startGame();

	// True if game should be restarted after close
	// False otherwise
	// this->startGame sets appropriate value
	return this->restartOnEnd;
}

void GameWindow::handleReveal(int x, int y)
{
	if(this->board.revealUnflagged(x,y))
	{
		this->board.buttonsToReveal--;

		if(this->board.buttonsToReveal == 0)
		{
			this->gameState = Logic::GameState::win; // WIN !
			this->gameStateMsg.setString("You win!");
		}

		if(this->board.get(x,y)->isMined())
		{
			this->gameState = Logic::GameState::lose; // lose
			this->gameStateMsg.setString("You have lost!");
			this->gameStateMsg.setFillColor({125,0,0});
		}

	}
}

int GameWindow::getMinesNumber() const
{
	auto ret = 0;
	for(int i = 0; i < this->board.height; i++)
	{
		for(int j = 0; j < this->board.width; j++)
		{
			if(this->board.get(j,i)->isMined())
				ret++;
		}
	}
	return ret;
}

int GameWindow::loadAssets()
{
	int ret = 0;
	char idx = '0';

	fprintf(stderr, "Loading textures . . . \n");

	for(auto& tex:this->numberTextures)
	{
		std::stringstream ss;
		ss << this->assetsPath.c_str() << idx << ".png";
		ret += !tex.loadFromFile(ss.str().c_str());
		++idx;
	}
	
	this->flagTexture.loadFromFile((this->assetsPath+"flag.png").c_str());

	return ret;
}

int GameWindow::loadFonts(const std::string &fontname)
{
	fprintf(stderr, "Loading fonts . . . \n");
	
	if(!this->mainFont.loadFromFile(fontname.c_str()))
	{
		puts("Error when loading font.");
		return 1;
	}

	return 0;
}

void GameWindow::flagButton(int xcoord, int ycoord)
{
	Field *f = this->board.get(xcoord, ycoord);
	
	if(f == nullptr)
		return;

	f->setFlagState(!f->isFlagged());
}

void GameWindow::drawBoard(sf::RenderWindow &wnd)
{
	this->drawHorizontalGrid(wnd);
	this->drawVerticalGrid(wnd);
	this->drawBoardButtons(wnd);
}

void GameWindow::randomPlay()
{
	while(!this->isGameOver())
	{
		int x = int(this->randomGen() % this->board.width);
		int y = int(this->randomGen() % this->board.height);

		sleep(1);
		
		this->handleReveal(x,y);
	}
	puts("GAMEOVER");
}

void GameWindow::drawBoardButtons(sf::RenderWindow &wnd)
{
	std::vector<sf::RectangleShape> gridButtons(this->board.width*this->board.height,
			sf::RectangleShape(sf::Vector2f(this->cellWidth-2, this->cellHeight-2)));
	
	Field *field;

	int gridOffset = 0;
	int minesCount = 0;

	for(int i=this->board.height-1; i>=0; --i)
	{
		for(int j=0; j<this->board.width; ++j)
		{
			gridButtons[(i*this->board.width)+j].setPosition(
					j*this->cellWidth+this->boardScreenXoffset+1,
					i*this->cellHeight+this->boardScreenYoffset+1);
			field = this->board.get(j,i);
			gridOffset = (i*this->board.width)+j;
			if(field->isCovered())
			{
				if(field->isFlagged())
				{
					gridButtons[gridOffset].setTexture(&this->flagTexture);
					continue;
				}

				gridButtons[gridOffset].setFillColor(sf::Color::Green);
				continue;
			}
			if(field->isMined())
			{
				gridButtons[gridOffset].setFillColor(sf::Color::Red);
				continue;
			}
			minesCount = this->board.countMines(j,i);
			gridButtons[gridOffset].setTexture(&this->numberTextures.at(minesCount));
		}
	}

	for(auto btn:gridButtons)
		wnd.draw(btn);
}

void GameWindow::drawHorizontalGrid(sf::RenderWindow &wnd)
{
	sf::VertexArray hLines(sf::Lines);
	
	register float xpoint;
	register float ypoint;

	sf::Color clr(255,255,255);
	if(this->gameState == Logic::GameState::lose)
	{
		clr.g = 0;
		clr.b = 0;
	}
	else if(this->gameState == Logic::GameState::win)
	{
		clr.r = 0;
		clr.g = 255;
		clr.b = 0;
	}

	for (int x=0; x<=this->board.height; x++)
	{
		xpoint = boardScreenXoffset;
		ypoint = boardScreenYoffset+(x*this->cellHeight);
		hLines.append(sf::Vertex(sf::Vector2f(xpoint, ypoint),clr)); // początek

		xpoint = boardScreenXoffset+(this->board.width*this->cellWidth);
		hLines.append(sf::Vertex(sf::Vector2f(xpoint, ypoint),clr)); // koniec
	};

	wnd.draw(hLines);
}

void GameWindow::drawVerticalGrid(sf::RenderWindow &wnd)
{
	sf::VertexArray vLines(sf::Lines);
	
	register float xpoint;
	register float ypoint;
	
	sf::Color clr(255,255,255);
	if(this->gameState == Logic::GameState::lose)
	{
		clr.g = 0;
		clr.b = 0;
	}
	else if(this->gameState == Logic::GameState::win)
	{
		clr.r = 0;
		clr.g = 255;
		clr.b = 0;
	}

	for (int x=0; x<=this->board.width; x++)
	{
		xpoint = boardScreenXoffset+(x*this->cellWidth);
		ypoint = boardScreenYoffset;
		vLines.append(sf::Vertex(sf::Vector2f(xpoint, ypoint),clr)); // początek

		ypoint = boardScreenYoffset+(this->board.height*this->cellHeight);
		vLines.append(sf::Vertex(sf::Vector2f(xpoint, ypoint),clr)); // koniec
	};

	wnd.draw(vLines);
}

void GameWindow::drawRestartButton
(sf::RenderWindow &wnd)
{
	wnd.draw(this->restartButton);
}

void GameWindow::drawGameStateMsg
(sf::RenderWindow &wnd)
{
	this->transformTextToWindowHeader(this->gameStateMsg);
	wnd.draw(this->gameStateMsg);
}

void GameWindow::transformTextToWindowHeader(sf::Text &txt)
{
	txt.setPosition(
		(this->windowWidth-txt.getGlobalBounds().width)/2,
		(int)this->windowHeight*0.05);
	while(txt.getGlobalBounds().width > (this->windowWidth*0.8))
	{
		txt.setCharacterSize(
			txt.getCharacterSize()/2);
			txt.setPosition(
				(this->windowWidth-txt.getGlobalBounds().width)/2,
				(int)this->windowHeight*0.05);
	}
}

size_t GameWindow::waitForButtonClick
(const std::vector<sf::Text> &btns, sf::RenderWindow &wnd)
{
	sf::Event event;
	size_t i = 0;
	while (wnd.isOpen())
	{
		while (wnd.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				wnd.close();

			if(event.type == sf::Event::MouseButtonPressed)
			{
				for(auto btn:btns)
				{
					if(btn.getGlobalBounds().contains(
								event.mouseButton.x,
								event.mouseButton.y))
						return i;
					++i;
				}
			}
		}
	}
	return SIZE_MAX;
}

size_t GameWindow::waitForButtonClick
(const sf::Text &btn, sf::RenderWindow &wnd)
{
	sf::Event event;
	while (wnd.isOpen())
	{
		while (wnd.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				wnd.close();

			if(event.type == sf::Event::MouseButtonPressed)
			{
				if(btn.getGlobalBounds().contains(
							event.mouseButton.x,
							event.mouseButton.y))
					return 0;
			}
		}
	}
	return SIZE_MAX; // window killed
}

void GameWindow::startGame()
{
	sf::RenderWindow 
		window(
				sf::VideoMode(this->windowWidth,this->windowHeight), 
				"SFML Sapper",
				sf::Style::Close
				);

	window.clear();

	// Start the game loop
	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window : exit
			if (event.type == sf::Event::Closed)
				window.close();

			if(isGameOver())
			{
				this->handleGameOver(window);
				return;
			}

			if(event.type == sf::Event::MouseButtonPressed)
			{
				// Out of width bound click
				if(event.mouseButton.x < this->boardScreenXoffset)
					continue;

				// Out of height bound click
				if(event.mouseButton.y < this->boardScreenYoffset)
					continue;

				int register xpressed =
								(int) ((event.mouseButton.x - this->boardScreenXoffset) / this->cellWidth);

				int register ypressed =
								(int) ((event.mouseButton.y - this->boardScreenYoffset) / this->cellHeight);

				if (event.mouseButton.button == sf::Mouse::Left)
				{
					this->handleReveal(xpressed, ypressed);
				}

				else if(event.mouseButton.button == sf::Mouse::Right)
				{
					this->flagButton(xpressed, ypressed);
				}
			}
		}

		// Draws main grid of board
		this->drawBoard(window);

		// Update the window
		window.display();
	}
}

void GameWindow::handleGameOver(sf::RenderWindow &wnd)
{
	this->drawGameStateMsg(wnd);
	this->handleRestart(wnd);
}

void GameWindow::handleRestart(sf::RenderWindow &wnd)
{
	this->drawRestartButton(wnd);
	wnd.display();

	this->restartOnEnd = Logic::EndGameState::restart;
	if(this->waitForButtonClick(this->restartButton, wnd))
		this->restartOnEnd = Logic::EndGameState::norestart;
}

void GameWindow::startGameConsole()
{
	int x,y;
	while(!this->isGameOver())
	{
		// Get user console input
		if(scanf("%i %i", &x, &y)!=2)
		// if input was invalid skip entered command line
		{
			while(getchar()!='\n');
			continue;
		}

		if(this->board.revealUnflagged(x,y))
		{
			this->board.buttonsToReveal--;


			if(this->board.buttonsToReveal == 0)
			{
				this->gameState = Logic::GameState::win; // WIN !
				this->gameStateMsg.setString("You win!");
			}

			if(this->board.get(x,y)->isMined())
			{
				this->gameState = Logic::GameState::lose; // lose
				this->gameStateMsg.setString("You have lost!");
				this->gameStateMsg.setFillColor({125,0,0});
			}

		}

		this->display();
	}
	puts("GAMEOVER!");
}
