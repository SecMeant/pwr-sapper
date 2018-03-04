#ifndef BOARD_H
#define BOARD_H

#include "Field.h"
#include <stdlib.h>
#include <time.h>

class Board
{
private:
	// Table of Width * Height fields
	Field *board;
	
	// Desribes span of mem that contains fields
	int boardWidth;
	int boardHeight;

	// Returns ptr to field from board
	// described with XY coordinates
	// Returns nullptr if coordinates are out of bound
	Field* getFromBoard(int x, int y) const;
public:
	// allocates memory for fields in 
	// size of width*height*sizeof(Field)
	Board(int width, int height);

	// Deallocates mem for fields
	~Board();

	// If random is set, fills board with randomly
	// placed bombs in number of n.
	// If random is not set fills top row,
	// left and right diagonal with bom (n is ignored).
	void deployMines(int n, bool random);

	void debug_display() const;
	
	// Changes field described by given XY coords to given values
	bool setField(int x, int y, bool mState, bool cState, bool fState);
};

#endif
