#ifndef LOGIC_H
#define LOGIC_H

class Logic
{
public:
	// pending : game is on !
	// gameover : user lost
	// win : user won
	enum class GameState {pending, lose, win};

	// Used to indicate wheter game should be 
	// restarted when current closes or not
	enum class EndGameState {restart, norestart};

	// user : user uses gui to play
	// random : computer reveal buttons randomly
	// console : user uses console to play
	enum class GameType {user, random, console};

};

#endif
