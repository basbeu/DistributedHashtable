/**
 * @file network.c
 * @brief 
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 20 Mar 2018
 */
#include <stdlib.h>
#include "network.h"
#include "config.h"
#include <string.h>

error_code send_request(node_t node, int socket, pps_key_t key, pps_value_t* value, size_t size_data){
	M_REQUIRE_NON_NULL(key);
	//M_REQUIRE_NON_NULL(value);
	
	error_code error = ERR_NONE;
                  
	ssize_t out_msg_len = sendto(socket, key, size_data, 0,(struct sockaddr *)&node.srv_addr, sizeof(node.srv_addr));
	
	char* temp_value = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));	
    ssize_t in_msg_len = recv(socket, temp_value, sizeof(value), 0);
	
	*value = temp_value;
	debug_print("%s", value);
	
	if (out_msg_len == -1 || in_msg_len == -1){
		error = ERR_NETWORK;
	}
	
	return error;
}

error_code network_get(client_t client, pps_key_t key, pps_value_t* value){
	//M_REQUIRE_NON_NULL(value);
	debug_print("%s", key);
	error_code err = ERR_NETWORK;
	for(size_t i = 0; i < client.list_servers->size && err != ERR_NONE; ++i){
		err = send_request(client.list_servers->list_of_nodes[i], client.socket, key, value, strlen(key));
		debug_print("%s", value);
	
	}

    return err;
}


char* prepare_msg(pps_key_t key, pps_value_t value, size_t* size_msg){
	size_t size_key = strlen(key);
	size_t size_value = strlen(value);
	*size_msg = size_key + size_value + 1;
	
	
	char* out_msg = calloc(*size_msg, sizeof(char));
	strncpy(out_msg, key, size_key);
	out_msg[size_key + 1] = '\0';
	strncpy(&out_msg[size_key+1], value, size_value);
	return out_msg;
}


error_code network_put(client_t client, pps_key_t key, pps_value_t value){
//	M_EXIT_IF_TOO_LONG(key, MAX_MSG_ELEM_SIZE, "");
//	M_EXIT_IF_TOO_LONG(value, MAX_MSG_ELEM_SIZE, "");
		
	char* out_msg=NULL;
	size_t size_msg = 0;
	out_msg = prepare_msg(key, value, &size_msg);
	
	error_code err = ERR_NONE;
	debug_print("%zu\n",client.list_servers->size);
	for(size_t i = 0; i < client.list_servers->size; ++i){
		error_code ans = send_request(client.list_servers->list_of_nodes[i], client.socket, out_msg, value, size_msg);
		if(ans != ERR_NONE){
			err = ans;
		} 
	}
	
	return err;
}
