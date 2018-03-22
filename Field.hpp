#ifndef FIELD_H
#define FIELD_H

#include <stdio.h>

class Field
{
private:
	// True - mine
	bool mineState;
	
	// True - flagged
	bool flagState;

	// True - covered
	bool coverState;

public:
	Field(): mineState(0), flagState(0), coverState(1){}
	Field(bool ms, bool fs, bool cs)
		:mineState(ms), flagState(fs), coverState(cs){}
	
	bool isMined() const;
	bool isFlagged() const;
	bool isCovered() const;

	void setMineState(bool st);
	void setFlagState(bool st);
	void setCoverState(bool st);
	
	void info() const;
};

#endif // FIELD_H
