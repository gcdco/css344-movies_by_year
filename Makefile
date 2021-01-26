CXX = gcc
CXXFLAGS = -std=gnu99 -g

ODIR=obj

OBJS = main.o movies.o

SRCS = main.c movies.c

HEADERS = movies.h

movies_by_year:
	${CXX} ${CXXFLAGS} ${SRCS} -o movies_by_year
	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ movies_by_year

removedir:
	rm -r duensing*

run:
	./movies_by_year
	
memcheck:
	valgrind --leak-check=yes --track-origins=yes --show-reachable=yes ./movies_by_year
