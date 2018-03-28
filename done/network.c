/**
 * @file network.c
 * @brief 
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 20 Mar 2018
 */
#include "network.h"

/*
error_code send_request(client_t client,pps_key_t* data, pps_value_t *value, size_t size_data){
	M_REQUIRE_NON_NULL(data);
	M_REQUIRE_NON_NULL(value);
	
	error_code error = ERR_NONE;
	
	ssize_t out_msg_len = sendto(client.socket, data, size_data, 0,(struct sockaddr *)&client.node.srv_addr, sizeof(client.node.srv_addr));
		
    ssize_t in_msg_len = recv(client.socket, value, sizeof(value), 0);
	
	if (out_msg_len == -1 || in_msg_len == -1){
		error = ERR_NETWORK;
	}
	
	return error;
}*/

error_code send_request2(node_t node, int socket, pps_key_t* data, pps_value_t *value, size_t size_data){
	M_REQUIRE_NON_NULL(data);
	M_REQUIRE_NON_NULL(value);
	
	error_code error = ERR_NONE;
                  
	ssize_t out_msg_len = sendto(socket, data, size_data, 0,(struct sockaddr *)&node.srv_addr, sizeof(node.srv_addr));
		
    ssize_t in_msg_len = recv(socket, value, sizeof(value), 0);
	
	if (out_msg_len == -1 || in_msg_len == -1){
		error = ERR_NETWORK;
	}
	
	return error;
}

error_code network_get(client_t client, pps_key_t key, pps_value_t *value){
	M_REQUIRE_NON_NULL(value);
	
	error_code err = ERR_NETWORK;
	for(size_t i = 0; i < client.list_servers->size && err != ERR_NONE; ++i){
		err = send_request2(client.list_servers->list_of_nodes[i], client.socket, &key, value, 1);
	
	}

    return err;
}

error_code network_put(client_t client, pps_key_t key, pps_value_t value){
	unsigned char out_msg[5];
	out_msg[0] = key;
		
	value = htonl(value);
		
	for(size_t i = 0; i < 4; ++i){
		out_msg[i+1] = (char) value;
		value >>= 8;
	}
	
	error_code err = ERR_NONE;
	debug_print("%zu\n",client.list_servers->size);
	for(size_t i = 0; i < client.list_servers->size; ++i){
		error_code ans = send_request2(client.list_servers->list_of_nodes[i], client.socket, out_msg, &value, 5);
		if(ans != ERR_NONE){
			err = ans;
		}   
	}
	
	return err;
}
