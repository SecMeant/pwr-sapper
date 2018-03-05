#include "Board.h"
#include "Field.h"

int main(void)
{
	int x,y;
	Board mainBoard(3,4);
	mainBoard.deployMines(3,1);
	mainBoard.debug_display();
	while(!mainBoard.isGameOver())
	{
		scanf(" %i %i",&x,&y);
		mainBoard.reveal(x,y);
		mainBoard.display();
	}
	puts("GAMEOVER!");
	return 0;
}
