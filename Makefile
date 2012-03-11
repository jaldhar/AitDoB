CXX=g++
CXXFLAGS=-std=c++0x -pedantic -O2 -g -Wall -Wextra $(shell ncurses5-config --cflags)
LDFLAGS=$(shell ncurses5-config --libs) -lstdc++
PREFIX=/usr/local

OBJECTS= aitdob.o \
         game.o \
         ui.o \
         world.o
      
aitdob: $(OBJECTS)
	$(CXX) $(OBJECTS) -o aitdob $(LDFLAGS)

install: aitdob
	install -oroot -groot -m0755 -s  alf $(PREFIX)/games

clean:
	-rm aitdob $(OBJECTS)
