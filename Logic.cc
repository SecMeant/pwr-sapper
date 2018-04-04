#include "Logic.hpp"

bool Logic::isGameOver() const
{
	if(this->gameState == Logic::GameState::pending)
		return false;
	return true;
}

int Logic::getMinesNumber(Board &board) const
{
	auto ret = 0;
	for(int i = 0; i < board.height; i++)
	{
		for(int j = 0; j < board.width; j++)
		{
			if(board.get(j,i)->isMined())
				ret++;
		}
	}
	return ret;
}

void Logic::deployMines(int n, bool random, Board &board)
{
	std::default_random_engine randomGen(time(NULL));
	Field *f;

	if(random)
	{

		// Calculate buttons to reveal
		board.buttonsToReveal = board.width*board.height;
		board.buttonsToReveal -= n;

		for(int i = 0; i < n; i++)
		{
			int xcoord = int(randomGen() % board.width);
			int ycoord = int(randomGen() % board.height);

			f = board.get(xcoord, ycoord);
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
	
	for(int i = 0, j = 0; i < board.width; i++, j++)
	{
		// Negative from bottom diagonal slope
		f = board.get(board.width-1-i, board.height-1-i);
		if(f!=nullptr)
			f->setMineState(true);
		
		// Positive from bottom diagonal slope
		f = board.get(i, board.height-1-j);
		if(f!=nullptr)
			f->setMineState(true);

		// Top row
		f = board.get(i, 0);
		if(f!=nullptr)
			f->setMineState(true);
	}

	// Calculate buttons to reveal
	board.buttonsToReveal = board.width*board.height;
	board.buttonsToReveal -= this->getMinesNumber(board);
	printf("%i",board.buttonsToReveal);
}

void Logic::handleReveal(int x, int y, Board &board)
{
	if(board.revealUnflagged(x,y))
	{
		board.buttonsToReveal--;

		if(board.buttonsToReveal == 0)
		{
			this->gameState = GameState::win;
		}

		if(board.get(x,y)->isMined())
		{
			this->gameState = GameState::lose;
		}

	}
}

void Logic::flagButton(int xcoord, int ycoord, Board &board)
{
	Field *f = board.get(xcoord, ycoord);
	
	if(f == nullptr)
		return;

	f->setFlagState(!f->isFlagged());
}

