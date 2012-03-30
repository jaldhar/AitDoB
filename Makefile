CXX=g++
CXXFLAGS=-std=c++0x -pedantic -O2 -g -Wall -Wextra $(shell ncurses5-config --cflags)
LDFLAGS=$(shell ncurses5-config --libs) -lstdc++
PREFIX=/usr/local

OBJECTS= aitdob.o \
         combat.o \
         door.o \
         game.o \
         player.o \
         room.o \
         thing.o \
         tile.o \
         ui.o \
         world.o
      
aitdob: $(OBJECTS)
	$(CXX) $(OBJECTS) -o aitdob $(LDFLAGS)

install: aitdob
	install -oroot -groot -m0755 -s  aitdob $(PREFIX)/games

clean:
	-rm aitdob $(OBJECTS)
