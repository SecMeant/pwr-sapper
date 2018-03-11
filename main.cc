#include "Board.h"
#include "Field.h"

int main(void)
{
	Board mainBoard(10,10);
	mainBoard.initStartGame(GameType::user);

	return 0;
}
