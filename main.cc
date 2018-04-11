#include "Sapper.hpp"

int main(void)
{
	Logic::EndGameState r = Logic::EndGameState::restart;
	while(r == Logic::EndGameState::restart)
	{
		GameWindow game;
		r = game.initStartGame();
	}

	return 0;
}
