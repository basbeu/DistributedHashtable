/**
 * @file pps-dump-node.c
 * @brief
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 25 Avr 2018
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h> 
 
#include "system.h"
#include "hashtable.h"
#include "node.h"
#include "config.h"
 
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
    
    //send the dump request
    sendto(socket, "/0", 1, 0, (struct sockaddr *) &node.srv_addr, sizeof(node.srv_addr));    
    
	char in_msg[MAX_MSG_SIZE];
    (void)memset(&in_msg, '\0', MAX_MSG_SIZE);
    socklen_t addr_len = sizeof(node.srv_addr);
    
    ssize_t in_msg_len = recvfrom(socket, &in_msg, sizeof(in_msg), 0,
                                      (struct sockaddr *)&node.srv_addr, &addr_len);
                                      
    char nb_pair_s[4];
    strncpy(nb_pair_s, in_msg, 4);                                  
    size_t nb_pair= atoi(nb_pair_s);
    size_t all_pair_size = nb_pair;
	kv_pair_t all_pair[all_pair_size];
    
    size_t index_pair = 0;
    size_t index_msg = 4;
    while(nb_pair > 0 && in_msg_len != -1){
		
		
		while(index_msg < in_msg_len){
			char* key = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
			char* value = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
			strncpy(key, &(in_msg[index_msg]),MAX_MSG_ELEM_SIZE);
			index_msg += strlen(key)+1;
			strncpy(value, &(in_msg[index_msg]),MAX_MSG_ELEM_SIZE);
			index_msg+= strlen(value)+1;
			
			all_pair[index_pair].key = key;
			all_pair[index_pair].value = value;
			
			--nb_pair;
			++index_pair;
		}
		in_msg_len = recvfrom(socket, &in_msg, sizeof(in_msg), 0,
                                      (struct sockaddr *)&node.srv_addr, &addr_len);
		index_msg = 0;
	}
	
	if(nb_pair != 0){
		printf("FAIL\n");
	}
	
	for(size_t i = 0; i < all_pair_size; ++i){
		printf("%s = %s\n",all_pair[i].key, all_pair[i].value);
	}                                                                        
    
    node_end(&node);
	return 0;
}
