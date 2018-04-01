#include "Board.hpp"

Board::Board(int w, int h): buttonsToReveal(0)
{
	if(w <= 0)
	{
		w = 1;
		puts("Board's width cannot be 0 or less!\n");
	}

	if(h <= 0)
	{
		h = 1;
		puts("Board's height cannot be 0 or less!\n");
	}

	this->width = w;
	this->height = h;

	this->board = new Field[w * h]();
}

Board::~Board()
{
	delete [] this->board;
}

Field * Board::get(int x, int y) const
{
	if(x >= this->width || x < 0)
	{
		return nullptr;
	}
	
	if(y >= this->height || y < 0)
	{
		return nullptr;
	}

	return this->board + (y*this->width) + x;
}

bool Board::setField(int x, int y, bool mState, bool cState, bool fState)
{
	Field *f = this->get(x,y);

	if(f == nullptr)
		return 0;

	f->setMineState(mState);
	f->setCoverState(cState);
	f->setFlagState(fState);

	return 1;
}

bool Board::hasMine(int x, int y) const
{
	Field *f = this->get(x,y);

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

bool Board::reveal(int x, int y)
{
	Field *f = this->get(x, y);

	if(f == nullptr)
		return false;

	if(!f->isCovered())
		return false;

	f->setCoverState(false);

	return true;
}

bool Board::revealUnflagged(int x, int y)
{
	Field *f = this->get(x, y);
	
	if(f == nullptr)
		return false;

	if(f->isFlagged())
		return false;

	return this->reveal(x,y);
}

