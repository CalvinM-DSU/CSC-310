INCLUDE_DIRS := Includes
CC 			 := g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

OBJS 		 := splay.o mainsplay.o
DEPS 		 := ${INCLUDE_DIRS}/splay.h

.PHONY: clean all

all: splay.o mainsplay.o splayTreeApp

clean:
	rm $(OBJS) splayTreeApp

splay.o: splay.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

mainsplay.o: mainsplay.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

splayTreeApp: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)