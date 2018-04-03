#include "Sapper.hpp"

int main(void)
{
	Logic::EndGameState r = Logic::EndGameState::restart;
	while(r == Logic::EndGameState::restart)
	{
		GameWindow game(4,4);
		r = game.initStartGame(4);
	}

	return 0;
}
