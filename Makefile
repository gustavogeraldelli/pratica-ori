CC = gcc
OBJ = obj
INCLDDE = include

all: main.o tabela_hash.o lista.o
	gcc $^ -o main

*.o: *.c
	gcc -c $^

clean:
	rm -rf *.o main