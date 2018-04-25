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
 
#define ADD_LENGTH 15
#define TIMEOUT 0
 
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
    
        
    
	return 0;
}
