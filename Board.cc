#include "Board.hpp"

Board::Board(int w, int h)
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

