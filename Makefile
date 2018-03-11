SHELL=cmd
cflags=-std=c++11
warningson=-Wall -Wextra
sfmllib=-I C:\SFML\include -I C:\SFML\lib
src=Main.cc Field.cc Board.cc

all:
	g++ $(src) $(cflags) $(sfmllib) -L $(wildcard C:\SFML\bin\*) $(warningson)
