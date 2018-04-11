#ifndef BOARD_H
#define BOARD_H

#include "Field.hpp"

class Board
{
private:
	// Table of Width * Height fields
	Field *board;

public:
	Board(int width, int height);
	Board(){this->board = nullptr;}
	~Board();

	// Reinitializes itself with new with and height
	void reinitialize(int w, int h);

	// Desribes span of mem that contains fields
	int width;
	int height;

	// Returns ptr to field from board
	// described with XY coordinates
	// Returns nullptr if coordinates are out of bound
	Field* get(int x, int y) const;

	// Holds value of buttons that must be revealed to win
	int buttonsToReveal;

	// Changes field described by given XY coords to given values
	bool setField(int x, int y, bool mState, bool cState, bool fState);

	// Returns true if field described by XY coords
	// contains bomb or not
	bool hasMine(int x, int y) const;

	// Returns number of mines around point
	// descibed as given XY coords
	int countMines(int x, int y) const;

	// Reveals filed descibed as given coords
	// Returns true if successufully revelad given field
	// False otherwise
	bool reveal(int x, int y);

	// same as reveal but first checks if button is flagged
	// if so does not reveals
	// Returns true if revealed button
	// false otherwise
	bool revealUnflagged(int x, int y);

};

#endif // BOARD_H
