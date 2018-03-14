LDLIBS = -lcheck -lm -lrt -pthread -lsubunit

all: week04 clean

week04: week04.o hashtable.o error.o
	gcc -o week04 week04.o $(LDLIBS)


hashtable.o: hashtable.c error.h hashtable.h
week04.o: week04.c tests.h hashtable.h
error.o: error.c error.h

clean :
	rm error.o hashtable.o week04.o
