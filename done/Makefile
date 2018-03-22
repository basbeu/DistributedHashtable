LDLIBS = -lcheck -lm -lrt -pthread -lsubunit
CFLAGS = -DDEBUG

all: week04 pps-client-put pps-client-get pps-launch-server clean

week04: week04.o hashtable.o error.o
	gcc -o week04 week04.o hashtable.o $(LDLIBS)

pps-client-put: pps-client-put.o system.o client.o network.o node.o
	gcc -o pps-client-put pps-client-put.o system.o client.o network.o node.o

pps-client-get: pps-client-get.o system.o client.o network.o node.o
	gcc -o pps-client-get pps-client-get.o system.o client.o network.o node.o

pps-launch-server: pps-launch-server.o system.o hashtable.o
	gcc -o pps-launch-server pps-launch-server.o system.o hashtable.o

pps-client-put.o: pps-client-put.c system.h client.h network.h node.h
pps-client-get.o: pps-client-get.c system.h client.h network.h node.h
pps-launch-server.o: pps-launch-server.c system.h config.h hashtable.h
system.o: system.c system.h error.h
client.o: client.c client.h system.h config.h
network.o: network.c network.h
node.o: node.c node.h system.h


hashtable.o: hashtable.c error.h hashtable.h
week04.o: week04.c tests.h hashtable.h
error.o: error.c error.h

clean :
	rm error.o hashtable.o week04.o pps-client-put.o pps-client-get.o pps-launch-server.o system.o client.o network.o node.o
