CXX?=g++
CXXFLAGS+=-std=c++0x -pedantic -O2 -g -Wall -Wextra $(shell ncurses5-config --cflags)
LDFLAGS+=$(shell ncurses5-config --libs) -lstdc++
INSTALL?=install
PREFIX?=/usr/local
GAMESDIR?=/games

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
	$(INSTALL) -d $(DESTDIR)$(PREFIX)$(GAMESDIR)
	$(INSTALL) -m0755 aitdob $(DESTDIR)$(PREFIX)$(GAMESDIR)

clean:
	-rm aitdob $(OBJECTS)
