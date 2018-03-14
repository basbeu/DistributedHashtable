LDLIBS = -lcheck -lm -lrt -pthread

all: hashtable week04

hashtable: hashtable.o error.o
	gcc -o hashtable hashtable.o error.o $(LDLIBS)
week04: week04.o
	gcc -o week04 week04.o $(LDLIBS)


hashtable.o: hashtable.c error.h hashtable.h
week04.o: week04.c tests.h hashtable.h
error.o: error.c error.h

clean :
	rm edit hashtable.o week04.o
