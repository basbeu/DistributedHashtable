/**
 * @file pps-launch-server.c
 * @brief
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 20 Mar 2018
 */

// standard includes (printf, exit, ...)
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // for errno
#include <string.h> // for memset()

// for basic socket communication
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "system.h"
#include "config.h"
#include "hashtable.h"
#include <inttypes.h> // for uint16_t
#include <string.h> // for memset()

#define ADD_LENGTH 15
#define HTABLE_LENGTH 1

void send_answer(int socket, pps_value_t response, size_t out_msg_len, const struct sockaddr_in* const cli_addr, socklen_t addr_len);

kv_pair_t decompose_msg(char* msg, size_t size_msg){
	size_t key_size = strlen(msg);
	char* key = calloc(key_size+1, sizeof(char));
	strncpy(key, msg, key_size);
	
	size_t value_size = size_msg - key_size;
	char* value = calloc(value_size + 1, sizeof(char));
	strncpy(value, &msg[key_size + 1], value_size);
	
	kv_pair_t pair;
	pair.key = key;
	pair.value = value;
	
	return pair;
}


int main(void){

	Htable_t table = construct_Htable(HTABLE_LENGTH);
	//(void)memset(&table, 0, sizeof(bucket_t)*HTABLE_SIZE);
	
	debug_print("size of : %zu", sizeof(table));
	//debug_print("size of : %zu", sizeof(bucket_t)*HTABLE_SIZE);
	
	int socket;
	size_t to = 0;
	socket = get_socket(to);
	if(socket == -1){
		debug_print("Unable to get a socket", 0);
		return 1;
	}
	
	char address[ADD_LENGTH];
	(void)memset(&address, 0, ADD_LENGTH);
	uint16_t port = 0;
	
	printf("IP port? ");
	int i = 0;
		int j = 0;
	do{
		i = scanf("%s", address);
		j = scanf("%" SCNu16, &port);
		while(!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');
	}while(i != 1 && j != 1);
	
	
	error_code err = bind_server(socket, address, port);
	if(err != ERR_NONE){
		debug_print("Unable to bind server", 0);
		return 1;
	}
	
	debug_print("\nListening on : %s, port : %" PRIu16 "\n", address, port);
	
	while(1){
		debug_print("Waiting for request", 0);
		struct sockaddr_in cli_addr;
        socklen_t addr_len = sizeof(cli_addr);
        memset(&cli_addr, 0, addr_len);
        char in_msg[MAX_MSG_SIZE];
        (void)memset(&in_msg, '\0', MAX_MSG_SIZE);
        ssize_t in_msg_len = recvfrom(socket, &in_msg, sizeof(in_msg), 0,
                                      (struct sockaddr *) &cli_addr, &addr_len);
         debug_print("received",0);
         /*pps_key_t key = in_msg[0];
         pps_value_t request = 0;
                               
         for(size_t i = 1; i < 5; ++i){
			request <<= 8;
			request = request|in_msg[i];
		 }*/
         pps_key_t key = NULL;
         pps_value_t value = NULL;
         kv_pair_t pair = decompose_msg(in_msg, in_msg_len); 
         
         debug_print("%s %s", pair.key, pair.value);
         
        //Declare response 
        //TODO init ???      
        pps_value_t response = NULL;
        
        //Declare size of response
        size_t out_msg_len = 0;
        
		//Writing request
		//if(in_msg_len == 5){		
		if(strlen(pair.value) != 0){
			//pps_value_t value = ntohl(request);
			add_Htable_value(table, pair.key, pair.value);
			send_answer(socket, response, out_msg_len, &cli_addr, addr_len);
		}
		
		//Reading request
		//else if(in_msg_len == 1){
		else{
			char* temp_response = get_Htable_value(table, pair.key);
			//response = get_Htable_value(table, pair.key);
			debug_print("%s", temp_response);
			//out_msg_len = sizeof(response);	
			if(temp_response != NULL){
				out_msg_len = strlen(temp_response);
			}else{
				out_msg_len = 1;
				temp_response = calloc(1, 1);
				//strncpy(temp_response, '\0', 0);
			}
			//response = temp_response;
			send_answer(socket, temp_response, out_msg_len, &cli_addr, addr_len);	
		}
		
		//Wrong request
		//else{
		//	debug_print("Wrong size of request, expected was 1 or 5, actual is %ld.\n", in_msg_len);
		//}
		
	}

	return 0;
}

void send_answer(int socket, pps_value_t response, size_t out_msg_len, const struct sockaddr_in* const cli_addr, socklen_t addr_len){
	//unsigned int out_msg = htonl(response);
	debug_print("%s", response);
	sendto(socket, response, out_msg_len, 0, (struct sockaddr *) cli_addr, addr_len);
}
