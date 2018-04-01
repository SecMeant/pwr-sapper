cflags=-std=c++11
warningson=-Wall -Wextra
sfmlso=-lsfml-graphics -lsfml-window -lsfml-system
sfmllib=-I C:\SFML\include -I C:\SFML\lib
solibs=-lpthread
src=main.cc Field.cc GameWindow.cc Board.cc
appname=game

all linux:
	g++ $(src) $(cflags) $(sfmlso) $(solibs) $(warningson) -o $(appname).out

windows:
	g++ $(src) $(cflags) $(sfmllib) $(sfmlso) $(warningson) -o $(appname).exe
