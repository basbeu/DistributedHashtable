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

int main(void){

	Htable_t table[HTABLE_SIZE];
	int socket;
	size_t to = 0;
	socket = get_socket(to);
	bind_server(socket, PPS_DEFAULT_IP, PPS_DEFAULT_PORT);
	
	while(1){
		struct sockaddr_in cli_addr;
        socklen_t addr_len = sizeof(cli_addr);
        memset(&cli_addr, 0, addr_len);
        unsigned long int in_msg;
        ssize_t in_msg_len = recvfrom(s, &in_msg, sizeof(in_msg), 0,
                                      (struct sockaddr *) &cli_addr, &addr_len);
                   
         unsigned long int request = ntohl(in_msg); 
         
        //Declare response       
        pps_value_t response = 0;
        
        //Declare size of response
        size_t out_msg_len = 0;
        
                
		//Writing request
		if(in_msg_len == 5){
			pps_key_t key = request>>32;
			pps_value_t value = request;
			add_Htable_value(table, key, value);
						
		}
		
		//Reading request
		else if(in_msg_len == 1){
			pps_key_t key = request;
			response = get_Htable_value(table, key);
			out_msg_len = sizeof(response);			
		}
		
		//Wrong request
		else{
			debug_print("Wrong size of request, expected was 1 or 5, actual is %d.\n", in_msg_len);
		}
		
		unsigned int out_msg = htonl(response);
		sendto(s, &out_msg, out_msg_len, 0, (struct sockaddr *) &cli_addr, addr_len);
			
	}

	return 0;
}
