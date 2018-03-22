#include "Board.hpp"

int main(void)
{
	char ch = 'y';
	while(ch == 'y' || ch == 'Y' || ch == '\n')
	{
		Board mainBoard(10,10);
		mainBoard.initStartGame(Board::GameType::user, 10);
		puts("Wanna restart ? (Y/n): ");
		ch = getchar();
		while(getchar() != '\n');
	}

	return 0;
}
