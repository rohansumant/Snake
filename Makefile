all:
	g++-7 -std=c++17 -lncurses -o snake *.cpp

debug:
	g++-7 -std=c++17 -lncurses -D DEBUG -o snake_debug *.cpp
