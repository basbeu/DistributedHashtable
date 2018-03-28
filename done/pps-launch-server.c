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

int main(void){

	Htable_t table;
	int socket;
	size_t to = 0;
	socket = get_socket(to);
	char address[ADD_LENGTH];
	(void)memset(&address, 0, ADD_LENGTH);
	uint16_t port = 0;
	
	printf("IP port? ");
	
	scanf("%s", address);
	scanf("%" SCNu16, &port);
	
	bind_server(socket, address, port);
	
	debug_print("\nListening on : %s, port : %" PRIu16 "\n", address, port);
	
	while(1){
		struct sockaddr_in cli_addr;
        socklen_t addr_len = sizeof(cli_addr);
        memset(&cli_addr, 0, addr_len);
        char in_msg[5];
        ssize_t in_msg_len = recvfrom(socket, &in_msg, sizeof(in_msg), 0,
                                      (struct sockaddr *) &cli_addr, &addr_len);
         
         pps_key_t key = in_msg[0];
         pps_value_t request = 0;
                               
         for(size_t i = 1; i < 5; ++i){
			request <<= 8;
			request = request|in_msg[i];
		 }
         
        //Declare response       
        pps_value_t response = 0;
        
        //Declare size of response
        size_t out_msg_len = 0;
                
		//Writing request
		if(in_msg_len == 5){		
			pps_value_t value = ntohl(request);
			add_Htable_value(table, key, value);	
		}
		
		//Reading request
		else if(in_msg_len == 1){
			response = get_Htable_value(table, key);
			out_msg_len = sizeof(response);			
		}
		
		//Wrong request
		else{
			debug_print("Wrong size of request, expected was 1 or 5, actual is %ld.\n", in_msg_len);
		}
		
		unsigned int out_msg = htonl(response);
		sendto(socket, &out_msg, out_msg_len, 0, (struct sockaddr *) &cli_addr, addr_len);	
	}

	return 0;
}
