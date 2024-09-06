all: main.o tabela_hash.o Set.o ArvoreAVL.o
	gcc $^ -o main

*.o: *.c
	gcc -c $^

clean:
	rm -rf *.o main