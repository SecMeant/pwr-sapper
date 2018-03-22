#include "Field.hpp"

bool Field::isMined() const {return this->mineState;}
bool Field::isFlagged() const {return this->flagState;}
bool Field::isCovered() const {return this->coverState;}

void Field::setMineState(bool st) {this->mineState = st;}
void Field::setFlagState(bool st) {this->flagState = st;}
void Field::setCoverState(bool st) {this->coverState = st;}

void Field::info() const
{
	putchar('[');
	this->isMined() ? putchar('1') : putchar('0');
	this->isCovered() ? putchar('1') : putchar('0');
	this->isFlagged() ? putchar('1') : putchar('0');
	putchar(']');
}
