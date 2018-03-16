#include "Board.h"
#include "Field.h"

int main(void)
{
	char ch = 'y';
	while(ch == 'y' || ch == 'Y' || ch == '\n')
	{
		Board mainBoard(3,7);
		mainBoard.initStartGame(Board::GameType::user);
		puts("Wanna restart ? (Y/n): ");
		ch = getchar();
		while(getchar() != '\n');
	}

	return 0;
}
