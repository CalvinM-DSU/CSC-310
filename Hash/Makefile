INCLUDE_DIRS := AVL
CC 			 := g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

OBJS 		 := hash.o mainhash.o avl.o
DEPS 		 := ${INCLUDE_DIRS}/avl.h hash.h

.PHONY: clean all

all: hash.o mainhash.o hashTableApp

clean:
	rm $(OBJS) hashTableApp

hash.o: hash.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

mainhash.o: mainhash.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

avl.o: ${INCLUDE_DIRS}/avl.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

hashTableApp: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)