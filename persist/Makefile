#I've been using essentially the same makefile throughout all my assignments,
#	but it just wasn't working for this one, so I made a more robust version after some research
CC      := g++
CFLAGS  := -std=c++17

SRCS    := p_bst_1.cpp p_bst_2.cpp p_queue.cpp p_bst_fat.cpp
OBJS    := $(SRCS:.cpp=.o)
EXES    := p_bst_1 p_bst_2 p_queue p_bst_fat

.PHONY: all clean

all: $(EXES)

$(EXES): %: %.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(EXES) $(OBJS)