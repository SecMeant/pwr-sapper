#include "Board.h"

inline void clearstdin()
{fseek(stdin,0,SEEK_END);}

Board::Board(int width, int height)
	:gameState(0), UIthread(nullptr), randomGen(time(NULL))
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

	this->board = new Field[width * height]();
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
		srand(time(NULL));
		for(int i = 0; i < n; i++)
		{
			int xcoord = rand() % this->boardWidth;
			int ycoord = rand() % this->boardHeight;

			f = this->getFromBoard(xcoord, ycoord);
			f->setMineState(1);
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
	for(int j = this->boardHeight-1; j >= 0; j--)
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
	ret += hasMine(x,y+1);   // top
	ret += hasMine(x+1,y+1); // top right

	ret += hasMine(x-1,y);   // left
	ret += hasMine(x+1,y);   // right

	ret += hasMine(x-1,y-1); // bottom left
	ret += hasMine(x,y-1);   // bottom
	ret += hasMine(x+1,y-1); // bottom right
	return ret;
}

void Board::display() const
{
	Field *f;
	int minesCount;
	for(int j = this->boardHeight-1; j >= 0; j--)
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
		return 0;

	if(!f->isCovered())
		return 0;

	f->setCoverState(0);
	
	if(f->isMined())
		this->gameState = 1; // GAMEOVER

	return 1;
}

bool Board::isGameOver()
{
	return this->gameState;
}

void Board::initStartGame(Board::GameType gt)
{
	this->deployMines(20,1);
	this->debug_display();
	if(gt == GameType::user)
	{
		this->UIthread = new std::thread(&Board::startGameConsole,this);
		this->startGame();
		return;
	}

	this->UIthread = new std::thread(&Board::randomPlay,this);
	this->startGame();
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
		int x = this->randomGen() % this->boardWidth;
		int y = this->randomGen() % this->boardHeight;

		sleep(1);
		
		this->reveal(x,y);
	}
	puts("GAMEOVER");
}

void Board::drawBoardButtons(sf::RenderWindow &wnd)
{
	std::vector<sf::RectangleShape> gridButtons(this->boardWidth*this->boardHeight,
			sf::RectangleShape(sf::Vector2f(this->cellWidth-2, this->cellHeight-2)));
	
	Field *field;

	for(int i=0; i<this->boardHeight; i++)
	{
		for(int j=0; j<this->boardWidth; j++)
		{
			gridButtons[(i*this->boardWidth)+j].setPosition(
					j*50+this->boardScreenYoffset+1,
					i*50+this->boardScreenXoffset+1);
			field = getFromBoard(j,i);
			if(field->isCovered())
			{
				gridButtons[(i*this->boardWidth)+j].setFillColor(sf::Color::Green);
				continue;
			}
			if(field->isMined())
			{
				gridButtons[(i*this->boardWidth)+j].setFillColor(sf::Color::Red);
				continue;
			}
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
	if(this->isGameOver())
	{
		clr.g = 0;
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
	if(this->isGameOver())
	{
		clr.g = 0;
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
		window(sf::VideoMode(this->windowWidth,this->windowHeight), "SFML Sapper");

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
		if(scanf(" %i %i",&x,&y) != 2)
			clearstdin();
		this->reveal(x,y);
		this->display();
	}
	puts("GAMEOVER!");
}
