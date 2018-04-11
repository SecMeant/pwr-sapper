cflags=-std=c++11
warningson=-Wall -Wextra
sfmlso=-lsfml-graphics -lsfml-window -lsfml-system
sfmldll=sfml-graphics-d-2.dll sfml-window-d-2.dll sfml-system-d-2.dll
sfmllib=-I C:\SFML\include -I C:\SFML\lib
solibs=-lpthread
src=main.cc Field.cc GameWindow.cc Board.cc Logic.cc Menu.cc tools.cc
appname=game

all linux:
	g++ $(src) $(cflags) $(sfmlso) $(solibs) $(warningson) -o $(appname).out

windows:
	g++ $(src) $(cflags) $(sfmllib) $(sfmldll) $(warningson) -o $(appname).exe
