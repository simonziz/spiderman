CPPFLAGS = -I/usr/include/irrlicht
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11 -Wno-unused-parameter

spiderman: main.o events.o character.o
	g++ $^ -o $@ -lIrrlicht

main.o events.o: events.h character.h
clean:
	@rm -f spiderman *.o
