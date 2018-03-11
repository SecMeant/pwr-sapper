SHELL=cmd
cflags=-std=c++11 -mwindows
warningson=-Wall -Wextra
sfmllib=-I C:\SFML\include -I C:\SFML\lib
sfmldlls=C:\SFML\bin\sfml-window-d-2.dll C:\SFML\bin\sfml-window-2.dll C:\SFML\bin\sfml-graphics-d-2.dll
sfmldlls+=C:\SFML\bin\sfml-graphics-2.dll C:\SFML\bin\sfml-system-2.dll C:\SFML\bin\sfml-system-d-2.dll
src=Main.cc Field.cc Board.cc

all:
	g++ $(src) $(cflags) $(sfmllib) $(sfmldlls) $(warningson)
