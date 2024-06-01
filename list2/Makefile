GCC      = gcc
CFLAGS   = -std=c99 -Wall -Werror -g3 -O3
OBJECTS  = list

MEMVAL   = valgrind
MEMFLAGS = --leak-check=yes --track-origins=yes -s

RM       = rm
RMFLAGS  = -rf

memcheck:
	$(MEMVAL) $(MEMFLAGS) ./$(OBJECTS).o

list:
	$(GCC) $(CFLAGS) list.c -o $(OBJECTS).o

clean:
	$(RM) $(RMFLAGS) *$(OBJECTS).o