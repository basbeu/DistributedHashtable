/**
 * @file pps-list-nodes.c
 * @brief
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 25 Apr 2018
 */
 
 #include <stdio.h>
 #include <stdlib.h>
 #include "system.h"
 
 #include "node_list.h"
 #include "client.h"
 #include "network.h"
 #include <inttypes.h> // for uint16_t
 #include "config.h"
 
 int main(void){
	
	node_list_t* list = get_nodes();
	ssize_t out_msg_len = 0;
	ssize_t in_msg_len = 0;
	int socket = get_socket(1);
	
	for(size_t i = 0; i < list->size; ++ i){
		

		node_t node = list->list_of_nodes[i];
		
		out_msg_len = sendto(socket, NULL, 0, 0, (struct sockaddr *) &node.srv_addr, sizeof(node.srv_addr));
		
		char* temp_value = calloc(1, sizeof(char));
		in_msg_len = recv(socket, temp_value, 1, 0);
		
		if(in_msg_len == 0){
			printf("%s %" PRIu16 " OK\n", node.ip, node.port);
		}
		else{
			printf("%s %" PRIu16 " FAIL\n", node.ip, node.port);
		}


		
		
	
	}
	
	
	
 
 
	return 0;
 }
