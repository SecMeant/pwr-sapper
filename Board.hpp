#ifndef BOARD_H
#define BOARD_H

#include "Field.hpp"

class Board
{
public:
	Board(int width, int height);
	~Board();

	// Table of Width * Height fields
	Field *board;

	// Desribes span of mem that contains fields
	int width;
	int height;

	// Returns ptr to field from board
	// described with XY coordinates
	// Returns nullptr if coordinates are out of bound
	Field* get(int x, int y) const;
};

#endif // BOARD_H
