#include <sstream>
#include "Board.hpp"

Board::Board(int width, int height)
	:buttonsToReveal(0), gameState(pending), UIthread(nullptr), randomGen(time(NULL)), assetsPath("./assets/")
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
}

Board::~Board()
{
	delete [] this->board;
}

void Board::deployMines(int n, bool random)
{
	Field *f;

	if(random)
	{
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
		// Positive diagonla slope
		this->getFromBoard(i, j)->setMineState(1);

		// Negative diagonal slope
		this->getFromBoard(this->boardWidth-1-i, this->boardHeight-1-i)->setMineState(1);

		// Top row
		this->getFromBoard(i, this->boardHeight-1)->setMineState(1);
	}

}

void Board::debug_display() const
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

Field * Board::getFromBoard(int x, int y) const
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

bool Board::setField(int x, int y, bool mState,bool cState, bool fState)
{
	Field *f = this->getFromBoard(x,y);

	if(f == nullptr)
		return 0;

	f->setMineState(mState);
	f->setCoverState(cState);
	f->setFlagState(fState);

	return 1;
}

bool Board::hasMine(int x, int y) const
{
	Field *f = getFromBoard(x,y);

	if(f == nullptr)
		return 0;
	return f->isMined();
}

int Board::countMines(int x, int y) const
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

void Board::display() const
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

bool Board::reveal(int x, int y)
{
	Field *f = getFromBoard(x, y);

	if(f == nullptr)
		return false;

	if(!f->isCovered())
		return false;

	f->setCoverState(false);

	this->buttonsToReveal--;

	if(this->buttonsToReveal == 0)
		this->gameState = win; // WIN !

	if(f->isMined())
		this->gameState = lose; // lose
 
	return true;
}

bool Board::revealUnflagged(int x, int y)
{
	Field *f = getFromBoard(x, y);
	
	if(f == nullptr)
		return false;

	if(f->isFlagged())
		return false;

	return this->reveal(x,y);
}

bool Board::isGameOver()
{
	if(this->gameState == pending)
		return false;
	return true;
}

Board::GameState Board::getGameState()
{
	return this->gameState;
}

void Board::initStartGame(Board::GameType gt, int minesCount)
{
	this->deployMines(minesCount,true);

	// Calculate buttons to reveal
	this->buttonsToReveal = this->boardWidth*this->boardHeight;
	this->buttonsToReveal -= minesCount;

	switch(gt)
	{
		case GameType::user:
			// just this->startGame
			break;
		case GameType::random:
			this->UIthread = new std::thread(&Board::randomPlay,this);
			break;
		case GameType::console:
			this->UIthread = new std::thread(&Board::startGameConsole,this);
			break;
		default:
			puts("Error! Wrong GameType.");
			return;
	}

	this->startGame();
}

int Board::loadAssets()
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

void Board::flagButton(int xcoord, int ycoord)
{
	Field *f = getFromBoard(xcoord, ycoord);
	f->setFlagState(!f->isFlagged());
}

void Board::drawBoard(sf::RenderWindow &wnd)
{
	this->drawHorizontalGrid(wnd);
	this->drawVerticalGrid(wnd);
	this->drawBoardButtons(wnd);
}

void Board::randomPlay()
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

void Board::drawBoardButtons(sf::RenderWindow &wnd)
{
	std::vector<sf::RectangleShape> gridButtons(this->boardWidth*this->boardHeight,
			sf::RectangleShape(sf::Vector2f(this->cellWidth-3, this->cellHeight-3)));
	
	Field *field;

	int gridOffset = 0;
	int minesCount = 0;

	for(int i=this->boardHeight-1; i>=0; --i)
	{
		for(int j=0; j<this->boardWidth; ++j)
		{
			gridButtons[(i*this->boardWidth)+j].setPosition(
					j*this->cellWidth+this->boardScreenXoffset+2,
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

void Board::drawHorizontalGrid(sf::RenderWindow &wnd)
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

void Board::drawVerticalGrid(sf::RenderWindow &wnd)
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

void Board::startGame()
{
	sf::RenderWindow 
		window(
				sf::VideoMode(this->windowWidth,this->windowHeight), 
				"SFML Sapper",
				sf::Style::Close
				);

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
				continue;

			if(event.type == sf::Event::MouseButtonPressed)
			{
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

		// Clear screen
		window.clear();

		// Draws main grid of board
		this->drawBoard(window);

		// Update the window
		window.display();
	}
}

void Board::startGameConsole()
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
