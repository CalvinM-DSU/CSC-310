INCLUDE_DIRS := Includes
CC 			 := g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

OBJS 		 := bst.o mainbst.o
DEPS 		 := ${INCLUDE_DIRS}/bst.h

.PHONY: clean all

all: bst.o mainbst.o searchTreeApp

clean:
	rm $(OBJS) searchTreeApp

bst.o: bst.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

mainbst.o: mainbst.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

searchTreeApp: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)