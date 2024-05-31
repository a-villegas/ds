GCC      = gcc
CFLAGS   = -std=c99 -Wall -Werror -g3 -O3
LFLAGS   =
OBJECT   = linkedlist

MEMVAL   = valgrind
MEMFLAGS = --leak-check=yes --track-origins=yes -s

RM       = rm
RMFLAGS  = -rf

memcheck:
	$(MEMVAL) $(MEMFLAGS) ./$(OBJECT) nums.txt

linkedlist: linked_list.c
	$(GCC) $(CFLAGS) $^ -o $(OBJECT)

clean:
	$(RM) $(RMFLAGS) $(OBJECT)