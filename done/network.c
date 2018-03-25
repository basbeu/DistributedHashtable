/**
 * @file network.c
 * @brief 
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 20 Mar 2018
 */
#include "network.h"

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
}

error_code network_get(client_t client, pps_key_t key, pps_value_t *value){
	M_REQUIRE_NON_NULL(value);

    return send_request(client, &key,value, 1);
}

error_code network_put(client_t client, pps_key_t key, pps_value_t value){
	unsigned char out_msg[5];
	out_msg[0] = key;
	
    value = htonl(value);
    
	for(size_t i = 0; i < 4; ++i){
		out_msg[i+1] = (char) value;
		value >>= 8;
	} 

	return send_request(client, out_msg, &value, 5);   
}
