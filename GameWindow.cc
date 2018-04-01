#include <sstream>
#include "GameWindow.hpp"

GameWindow::GameWindow(int width, int height)
:buttonsToReveal(0), restartOnEnd(norestart), gameState(pending),
 randomGen(time(NULL)), assetsPath("./assets/")
{
	if(width <= 0)
	{
		width = 1;
		puts("Board's width cannot be 0 or less!\n");
	}

	if(height <= 0)
	{
		height = 1;
		puts("Board's height cannot be 0 or less!\n");
	}

	this->boardWidth = width;
	this->boardHeight = height;

	this->windowWidth = int(boardScreenXoffset*2 + width*cellWidth);
	this->windowHeight = int(boardScreenYoffset*2 + height*cellHeight);

	this->board = new Field[width * height]();
	
	this->loadAssets();
	this->loadFonts(defaultFont);

	{ // deletes helper variables
		this->restartButton.setString("Restart");
		this->restartButton.setFont(mainFont);
		this->restartButton.setCharacterSize(restartButtonFontSize);
		this->restartButton.setColor({255,0,255});
		
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
		this->gameStateMsg.setColor({0,255,0});

		int gameStateMsgWidth = this->gameStateMsg.getGlobalBounds().width;
		register int xoffRB = (this->windowWidth-gameStateMsgWidth)/2;
		register int yoffRB = 0;
		
		this->gameStateMsg.setPosition(xoffRB,yoffRB);
	}
}

GameWindow::~GameWindow()
{
	delete [] this->board;
}

void GameWindow::deployMines(int n, bool random)
{
	Field *f;

	if(random)
	{

		// Calculate buttons to reveal
		this->buttonsToReveal = this->boardWidth*this->boardHeight;
		this->buttonsToReveal -= n;

		for(int i = 0; i < n; i++)
		{
			int xcoord = int(this->randomGen() % this->boardWidth);
			int ycoord = int(this->randomGen() % this->boardHeight);

			f = this->getFromBoard(xcoord, ycoord);
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
	
	for(int i = 0, j = 0; i < this->boardWidth; i++, j++)
	{
		// Negative from bottom diagonal slope
		f = this->getFromBoard(this->boardWidth-1-i, this->boardHeight-1-i);
		if(f!=nullptr)
			f->setMineState(true);
		
		// Positive from bottom diagonal slope
		f = this->getFromBoard(i, this->boardHeight-1-j);
		if(f!=nullptr)
			f->setMineState(true);

		// Top row
		f = this->getFromBoard(i, 0);
		if(f!=nullptr)
			f->setMineState(true);
	}

	// Calculate buttons to reveal
	this->buttonsToReveal = this->boardWidth*this->boardHeight;
	this->buttonsToReveal -= this->getMinesNumber();
	printf("%i",this->buttonsToReveal);
}

void GameWindow::debug_display() const
{
	for(int j = 0; j < this->boardHeight; j++)
	{
		for(int i = 0; i < this->boardWidth; i++)
		{
			this->getFromBoard(i, j)->info();
		}
		putchar('\n');
	}
}

Field * GameWindow::getFromBoard(int x, int y) const
{
	if(x >= this->boardWidth || x < 0)
	{
		return nullptr;
	}
	
	if(y >= this->boardHeight || y < 0)
	{
		return nullptr;
	}

	return this->board + (y*this->boardWidth) + x;
}

bool GameWindow::setField(int x, int y, bool mState,bool cState, bool fState)
{
	Field *f = this->getFromBoard(x,y);

	if(f == nullptr)
		return 0;

	f->setMineState(mState);
	f->setCoverState(cState);
	f->setFlagState(fState);

	return 1;
}

bool GameWindow::hasMine(int x, int y) const
{
	Field *f = getFromBoard(x,y);

	if(f == nullptr)
		return 0;
	return f->isMined();
}

int GameWindow::countMines(int x, int y) const
{
	int ret = 0;
	ret += hasMine(x-1,y+1); // top left
	ret += hasMine(x,y+1);	 // top
	ret += hasMine(x+1,y+1); // top right

	ret += hasMine(x-1,y);	 // left
	ret += hasMine(x+1,y);	 // right

	ret += hasMine(x-1,y-1); // bottom left
	ret += hasMine(x,y-1);	 // bottom
	ret += hasMine(x+1,y-1); // bottom right
	return ret;
}

void GameWindow::display() const
{
	Field *f;
	int minesCount;
	for(int j = 0; j < this->boardHeight; j++)
	{
		for(int i = 0; i < this->boardWidth; i++)
		{
			f = this->getFromBoard(i,j);
			minesCount = this->countMines(i,j);
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

bool GameWindow::reveal(int x, int y)
{
	Field *f = getFromBoard(x, y);

	if(f == nullptr)
		return false;

	if(!f->isCovered())
		return false;

	f->setCoverState(false);

	this->buttonsToReveal--;

	if(this->buttonsToReveal == 0)
	{
		this->gameState = win; // WIN !
		this->gameStateMsg.setString("You win!");
	}

	if(f->isMined())
	{
		this->gameState = lose; // lose
		this->gameStateMsg.setString("You have lost!");
		this->gameStateMsg.setColor({125,0,0});
	}

	return true;
}

bool GameWindow::revealUnflagged(int x, int y)
{
	Field *f = getFromBoard(x, y);
	
	if(f == nullptr)
		return false;

	if(f->isFlagged())
		return false;

	return this->reveal(x,y);
}

bool GameWindow::isGameOver()
{
	if(this->gameState == pending)
		return false;
	return true;
}

GameWindow::GameState GameWindow::getGameState()
{
	return this->gameState;
}

GameWindow::EndGameState
GameWindow::initStartGame(int minesCount)
{
	this->deployMines(minesCount,true);
	this->startGame();

	// True if game should be restarted after close
	// False otherwise
	// this->startGame sets appropriate value
	return this->restartOnEnd;
}

int GameWindow::getMinesNumber() const
{
	auto ret = 0;
	for(int i = 0; i < this->boardHeight; i++)
	{
		for(int j = 0; j < this->boardWidth; j++)
		{
			if(this->getFromBoard(j,i)->isMined())
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
	Field *f = getFromBoard(xcoord, ycoord);
	
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
		int x = int(this->randomGen() % this->boardWidth);
		int y = int(this->randomGen() % this->boardHeight);

		sleep(1);
		
		this->reveal(x,y);
	}
	puts("GAMEOVER");
}

void GameWindow::drawBoardButtons(sf::RenderWindow &wnd)
{
	std::vector<sf::RectangleShape> gridButtons(this->boardWidth*this->boardHeight,
			sf::RectangleShape(sf::Vector2f(this->cellWidth-2, this->cellHeight-2)));
	
	Field *field;

	int gridOffset = 0;
	int minesCount = 0;

	for(int i=this->boardHeight-1; i>=0; --i)
	{
		for(int j=0; j<this->boardWidth; ++j)
		{
			gridButtons[(i*this->boardWidth)+j].setPosition(
					j*this->cellWidth+this->boardScreenXoffset+1,
					i*this->cellHeight+this->boardScreenYoffset+1);
			field = getFromBoard(j,i);
			gridOffset = (i*this->boardWidth)+j;
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
			minesCount = this->countMines(j,i);
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
	if(this->gameState == lose)
	{
		clr.g = 0;
		clr.b = 0;
	}
	else if(this->gameState == win)
	{
		clr.r = 0;
		clr.g = 255;
		clr.b = 0;
	}

	for (int x=0; x<=this->boardHeight; x++)
	{
		xpoint = boardScreenXoffset;
		ypoint = boardScreenYoffset+(x*this->cellHeight);
		hLines.append(sf::Vertex(sf::Vector2f(xpoint, ypoint),clr)); // początek

		xpoint = boardScreenXoffset+(this->boardWidth*this->cellWidth);
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
	if(this->gameState == lose)
	{
		clr.g = 0;
		clr.b = 0;
	}
	else if(this->gameState == win)
	{
		clr.r = 0;
		clr.g = 255;
		clr.b = 0;
	}

	for (int x=0; x<=this->boardWidth; x++)
	{
		xpoint = boardScreenXoffset+(x*this->cellWidth);
		ypoint = boardScreenYoffset;
		vLines.append(sf::Vertex(sf::Vector2f(xpoint, ypoint),clr)); // początek

		ypoint = boardScreenYoffset+(this->boardHeight*this->cellHeight);
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
					this->revealUnflagged(xpressed, ypressed);
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

	this->restartOnEnd = GameWindow::EndGameState::restart;
	if(this->waitForButtonClick(this->restartButton, wnd))
		this->restartOnEnd = GameWindow::EndGameState::norestart;
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

		this->revealUnflagged(x,y);
		this->display();
	}
	puts("GAMEOVER!");
}
