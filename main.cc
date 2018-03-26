#include "Board.hpp"

int main(void)
{
	Board::EndGameState r = Board::EndGameState::restart;
	while(r == Board::EndGameState::restart)
	{
		Board mainBoard(4,4);
		r = mainBoard.initStartGame(Board::GameType::user, 4);
	}

	return 0;
}
