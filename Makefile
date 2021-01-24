CC = gcc
CFLAGS = -g -Wall --std=gnu99

movies_by_year: main.o
	$(CC) $(CFLAGS) -o $@ $^

movies.o : movies.c

memCheckSpellChecker :
	valgrind --tool=memcheck --leak-check=yes movies_by_year

clean :
	-rm *.o
	-rm movies_by_year

memory:
	valgrind --leak-check=yes --show-reachable=yes ./movies_by_year movies.csv

run:
	./movies_by_year movies.csv
