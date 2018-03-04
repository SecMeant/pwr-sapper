#include "Board.h"
#include "Field.h"

int main(void)
{
	Board mainBoard(3,4);
	mainBoard.setField(2,4,1,1,1);
	mainBoard.setField(0,1,0,1,1);
	mainBoard.debug_display();

	return 0;
}
