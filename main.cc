#include "GameWindow.hpp"

int main(void)
{
	GameWindow::EndGameState r = GameWindow::EndGameState::restart;
	while(r == GameWindow::EndGameState::restart)
	{
		GameWindow game(4,4);
		r = game.initStartGame(4);
	}

	return 0;
}
