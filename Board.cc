#include "Board.h"

inline void clearstdin()
{fseek(stdin,0,SEEK_END);}

Board::Board(int width, int height): gameState(0)
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

void Board::initStartGame()
{
	this->deployMines(3,1);
	this->startGame();
}

void Board::drawBoard(sf::RenderWindow &wnd)
{
	sf::VertexArray linesarray(sf::Lines);

	this->drawHorizontalGrid(linesarray);
	this->drawVerticalGrid(linesarray);

	wnd.draw(linesarray);
}

void Board::drawHorizontalGrid(sf::VertexArray &larray)
{
	register float xpoint;
	register float ypoint;

	for (int x=0; x<=this->boardHeight; x++)
	{
		xpoint = boardScreenXoffset;
		ypoint = boardScreenYoffset+(x*this->cellHeight);
		larray.append(sf::Vector2f(xpoint, ypoint)); // początek

		xpoint = boardScreenXoffset+(this->boardWidth*this->cellWidth);
		larray.append(sf::Vector2f(xpoint, ypoint)); // koniec
	};
}

void Board::drawVerticalGrid(sf::VertexArray &larray)
{
	register float xpoint;
	register float ypoint;

	for (int x=0; x<=this->boardWidth; x++)
	{
		xpoint = boardScreenXoffset+(x*this->cellWidth);
		ypoint = boardScreenYoffset;
		larray.append(sf::Vector2f(xpoint, ypoint)); // początek

		ypoint = boardScreenYoffset+(this->boardHeight*this->cellHeight);
		larray.append(sf::Vector2f(xpoint, ypoint)); // koniec
	};
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
