LDLIBS = -lcheck -lm -lrt -pthread -lsubunit
CFLAGS = -DDEBUG

#all: week04 pps-client-put pps-client-get pps-launch-server clean
all: pps-client-put pps-client-get pps-client-cat pps-client-find pps-client-substr pps-launch-server pps-dump-node pps-list-nodes clean

#week04: week04.o hashtable.o error.o 
#	gcc -o week04 week04.o hashtable.o $(LDLIBS)

pps-client-put: pps-client-put.o system.o client.o network.o node.o node_list.o error.o hashtable.o args.o
	gcc -o pps-client-put pps-client-put.o system.o client.o network.o node.o node_list.o error.o hashtable.o args.o

pps-client-get: pps-client-get.o system.o client.o network.o node.o node_list.o hashtable.o args.o
	gcc -o pps-client-get pps-client-get.o system.o client.o network.o node.o node_list.o hashtable.o args.o

pps-client-cat: pps-client-cat.o system.o client.o network.o node.o node_list.o hashtable.o args.o
	gcc -o pps-client-cat pps-client-cat.o system.o client.o network.o node.o node_list.o hashtable.o args.o

pps-client-find: pps-client-find.o system.o client.o network.o node.o node_list.o hashtable.o args.o
	gcc -o pps-client-find pps-client-find.o system.o client.o network.o node.o node_list.o hashtable.o args.o

pps-client-substr: pps-client-substr.o system.o client.o network.o node.o node_list.o hashtable.o args.o
	gcc -o pps-client-substr pps-client-substr.o system.o client.o network.o node.o node_list.o hashtable.o args.o

pps-launch-server: pps-launch-server.o system.o hashtable.o
	gcc -o pps-launch-server pps-launch-server.o system.o hashtable.o
	
pps-dump-node: pps-dump-node.o system.o hashtable.o node.o client.o args.o node_list.o
	gcc -o pps-dump-node pps-dump-node.o system.o hashtable.o node.o client.o args.o node_list.o

pps-list-nodes: pps-list-nodes.o system.o client.o network.o node_list.o node.o hashtable.o args.o
	gcc -o pps-list-nodes pps-list-nodes.o system.o client.o network.o node_list.o node.o hashtable.o args.o


clean :
	rm error.o hashtable.o pps-client-put.o pps-client-get.o pps-launch-server.o system.o client.o network.o node.o node_list.o pps-dump-node.o pps-list-nodes.o pps-client-cat.o pps-client-substr.o
