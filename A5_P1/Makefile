INCLUDE_DIRS := Includes
CC 			 := g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

OBJS 		 := binomial.o main.o customErrorClass.o
DEPS 		 := ${INCLUDE_DIRS}/binomial.h ${INCLUDE_DIRS}/customErrorClass.h

.PHONY: clean all

all: binomial.o main.o binomialApp

clean:
	rm $(OBJS) binomialApp

binomial.o: binomial.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

main.o: main.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

customErrorClass.o: customErrorClass.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

binomialApp: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)