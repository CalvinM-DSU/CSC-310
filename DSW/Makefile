INCLUDE_DIRS := Includes
CC 			 := g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

OBJS 		 := dsw.o main.o
DEPS 		 := ${INCLUDE_DIRS}/dsw.h

.PHONY: clean all

all: dsw.o main.o dswTreeApp

clean:
	rm $(OBJS) dswTreeApp

dsw.o: dsw.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

mainsplay.o: main.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

dswTreeApp: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)