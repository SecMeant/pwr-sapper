#include "Board.h"
#include "Field.h"

int main(void)
{
	Board mainBoard(3,4);
	mainBoard.setField(0,1,1,0,0);
	mainBoard.setField(0,2,1,0,0);
	mainBoard.setField(2,1,1,0,0);
	mainBoard.setField(2,0,1,0,0);
	mainBoard.setField(2,3,1,0,0);
	mainBoard.debug_display();
	printf("%i\n",mainBoard.countMines(1,1));

	return 0;
}
