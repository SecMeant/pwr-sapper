cflags=-std=c++11
warningson=-Wall -Wextra
sfmlso=-lsfml-graphics -lsfml-window -lsfml-system
solibs=-lpthread
src=main.cc Field.cc Board.cc
appname=game.out

all:
	g++ $(src) $(cflags) $(sfmllib) $(sfmlso) $(solibs) $(warningson) -o $(appname)
