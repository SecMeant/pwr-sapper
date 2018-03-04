#include "Board.h"

Board::Board(int width, int height): boardWidth(width), boardHeight(height)
{
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

			printf("Random: %i %i\n", xcoord, ycoord);
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
		printf("Out of bound: %i %i.\n",x,y);
		return nullptr;
	}
	
	if(y >= this->boardHeight || y < 0)
	{
		printf("Out of bound: %i %i.\n",x,y);
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


