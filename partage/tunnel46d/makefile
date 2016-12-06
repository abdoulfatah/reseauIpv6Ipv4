# Makefile créant l'executable "tunnel46d"

CC = gcc
FLAGS = 
EXEC = "tunnel46d"

all :: $(EXEC)

$(EXEC) : main.o extremite.o arguments.o
	$(CC) $(FLAGS) -o $(EXEC) main.o extremite.o arguments.o

main.o : extremite.o arguments.o main.c
	$(CC) $(FLAGS) -c main.c

extremite.o : extremite.c extremite.h
	$(CC) $(FLAGS) -c extremite.c

arguments.o : arguments.c arguments.h
	$(CC) $(FLAGS) -c arguments.c

clean::
	rm -f *.o $(EXEC)





