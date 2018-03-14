all: hashtable week04

hashtable: hashtable.o
	gcc -o hashtable hashtable.o
week04: week04.o
	gcc -o week04 week04.o


hashtable.o: hashtable.c error.h hashtable.h
week04.o: week04.c tests.h hashtable.h

clean :
	rm edit hashtable.o week04.o
