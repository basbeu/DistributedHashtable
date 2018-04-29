/**
 * @file pps-dump-node.c
 * @brief
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 25 Avr 2018
 */
 
#include <stdio.h>
#include <inttypes.h> 
 
#include "system.h"
#include "node.h"
 
#define ADD_LENGTH 15
#define TIMEOUT 1
 
 int main(void)
{
	int socket = get_socket(TIMEOUT);
    if(socket == -1) {
        debug_print("Unable to get a socket", 0);
        return 1;
    }

    char address[ADD_LENGTH];
    (void)memset(&address, 0, ADD_LENGTH);
    uint16_t port = 0;
	
	int i = 0;
    int j = 0;
    do {
        i = scanf("%s", address);
        j = scanf("%" SCNu16, &port);
        while(!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');
    } while(i != 1 && j != 1);
    
    node_t node;
    node_init(&node, address, port, 0);
    
    sendto(socket, "/0", 1, 0, (struct sockaddr *) &node.srv_addr, sizeof(node.srv_addr));    
    
    
    node_end(&node);
	return 0;
}
